import bpy, bmesh
from mathutils import Vector

_FPS_FACTOR = 1

class LayerData:
    def __init__(self, name, componentType, count, layer, attrName = ""):
        self.name = name
        self.type = componentType
        self.count = count
        self.layer = layer
        self.data = list()
        self.attrName = attrName
    def __str__(self):
        return f'{self.name} ({len(self.data)}):\n{self.data}'
    def ReadDataVerts(self,bm:bmesh.types.BMesh):
        ""
        for face in bm.faces:
            for loop in face.loops:
                data = loop.vert[self.layer]
                if self.attrName:
                    data = getattr(data, self.attrName)
                self.data.append(data.to_tuple())
    def ReadDataLoops(self,bm:bmesh.types.BMesh):
        ""
        for face in bm.faces:
            for loop in face.loops:
                data = loop[self.layer]
                if self.attrName:
                    data = getattr(data, self.attrName)
                self.data.append(data.to_tuple())
    def ExportJSON(self):
        ""
        # Header
        jsonOut = '\n    {'
        # Data
        jsonOut += f'\n      "Name": "{self.name}",'
        jsonOut += f'\n      "Type": "{self.type}",'
        jsonOut += f'\n      "Count": {self.count},'
        jsonOut += f'\n      "Data": ['
        for data in self.data:
            jsonOut += '\n        ' + ''.join(f'{value},' for value in data)
        jsonOut = jsonOut.rstrip(',')
        jsonOut += f'\n      ]'
        # Footer
        jsonOut = jsonOut.rstrip(',')
        jsonOut += '\n    }'
        return jsonOut
    @classmethod
    def ReadDataVertsPosition(cls,bm:bmesh.types.BMesh):
        ""
        positions = cls("Position", "Single", 3, None)
        for face in bm.faces:
            for loop in face.loops:
                data = loop.vert.co
                positions.data.append(data.to_tuple())
        return positions
    @classmethod
    def ReadDataVertsNormal(cls,bm:bmesh.types.BMesh):
        ""
        # TODO: Get split normals!
        normals = cls("Normal", "Single", 3, None)
        for face in bm.faces:
            for loop in face.loops:
                data = loop.vert.normal
                normals.data.append(data.to_tuple())
        return normals
    @classmethod
    def ReadDataVertsDeform(cls,bm:bmesh.types.BMesh,layer:bmesh.types.BMLayerItem):
        ""
        MAX_BINDS = 4
        indices = cls("DeformIndex", "UInt", 4, layer)
        weights = cls("DeformWeight", "Single", 4, layer)
        for face in bm.faces:
            for loop in face.loops:
                dataRaw = loop.vert[layer]
                data = sorted(dataRaw.items(), key=lambda kv:kv[1], reverse=True)[:MAX_BINDS]
                data += [(-1,0.0)] * (MAX_BINDS-len(data))
                indices.data.append(tuple(kv[0]+1 for kv in data))
                weightSum = sum(kv[1] for kv in data)
                weights.data.append(tuple(kv[1]/weightSum for kv in data))
        return indices, weights

def GetData(bm:bmesh.types.BMesh):
    ""
    layerDatas = list()
    ldPos = LayerData.ReadDataVertsPosition(bm)
    layerDatas.append(ldPos)
    ldNrm = LayerData.ReadDataVertsNormal(bm)
    layerDatas.append(ldNrm)
    def GetLayers(layers:bmesh.types.BMLayerCollection, isVert:bool, type:str, count:int, attrName:str=""):
        for layerName, layer in layers.items():
            ld = LayerData(layerName, type, count, layer, attrName)
            if isVert:
                ld.ReadDataVerts(bm)
            else:
                ld.ReadDataLoops(bm)
            layerDatas.append(ld)
    vLayers = bm.verts.layers
    lLayers = bm.loops.layers
    GetLayers(vLayers.float_vector, True, "Single", 3)
    GetLayers(vLayers.float_color, True, "Single", 4)
    GetLayers(vLayers.color, True, "Single", 4)
    GetLayers(lLayers.uv, False, "Single", 2, "uv")
    GetLayers(lLayers.float_vector, False, "Single", 3)
    GetLayers(lLayers.float_color, False, "Single", 4)
    GetLayers(lLayers.color, False, "Single", 4)
    for layer in bm.verts.layers.deform.values():
        ldI, ldW = LayerData.ReadDataVertsDeform(bm, layer)
        layerDatas.append(ldI)
        layerDatas.append(ldW)
    # print(layerDatas)
    return layerDatas

def RemoveDuplicates(layerDatas:list[LayerData]):
    ""
    baseCount = len(layerDatas[0].data)
    for layerData in layerDatas[1:]:
        assert len(layerData.data) == baseCount
    indices = [-1] * baseCount
    kvs = dict()
    for i in range(baseCount):
        loopData = tuple(layerData.data[i] for layerData in layerDatas)
        kvs.setdefault(loopData, len(kvs))
        indices[i] = kvs[loopData]
    for i, layerData in enumerate(layerDatas):
        layerData.data = list(k[i] for k in kvs.keys())
    return indices

def ExportAttributes(bm:bmesh.types.BMesh, layerDatas:list[LayerData]):
    ""
    jsonOut = ""
    for layerData in layerDatas:
        jsonOut += layerData.ExportJSON() + ","
    return jsonOut

def ExportPrimitives(bm:bmesh.types.BMesh, indices:list[int]):
    ""
    # Header
    jsonOut = '\n    {'
    # Data
    jsonOut += '\n      "Type": "Triangles",'
    jsonOut += '\n      "Indices": ['
    loopTotal = 0
    for face in bm.faces:
        face.loops.index_update()
        jsonOut += '\n        ' + ''.join(f'{indices[loopTotal + loop.index]},' for loop in face.loops)
        loopTotal += len(face.loops)
    jsonOut = jsonOut.rstrip(',\n')
    jsonOut += '\n      ],'
    # Footer
    jsonOut = jsonOut.rstrip(',')
    jsonOut += '\n    },'
    return jsonOut

def ExportMesh(mesh:bpy.types.Mesh, name:str):
    ""
    # Header
    jsonOut = '{\n  "Mesh": {'
    # Data
    jsonOut += '\n    "Name": "' + name + '",'
    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.verts.ensure_lookup_table()
    bm.verts.index_update()
    bm.edges.ensure_lookup_table()
    bm.edges.index_update()
    bm.faces.ensure_lookup_table()
    bm.faces.index_update()
    for face in bm.faces:
        face.loops.index_update()
    layerDatas = GetData(bm)
    indices = RemoveDuplicates(layerDatas)
    jsonOut += f'\n    "VertCount": {max(indices) + 1},'
    # Attributes
    jsonOut += '\n    "Attributes": ['
    jsonOut += ExportAttributes(bm, layerDatas)
    jsonOut = jsonOut.rstrip(',')
    jsonOut += '\n    ],'
    # Primitives
    jsonOut += '\n    "Primitives": ['
    jsonOut += ExportPrimitives(bm, indices)
    jsonOut = jsonOut.rstrip(',')
    jsonOut += '\n    ],'
    bm.free()
    # Footer
    jsonOut = jsonOut.rstrip(',')
    jsonOut += '\n  }\n}'
    return jsonOut

def ExportBone(bone:bpy.types.Bone):
    ""
    # Header
    jsonOut = '\n    {'
    # Data
    jsonOut += f'\n      "Name": "{bone.name}",'
    mtx = bone.matrix_local
    if bone.parent:
        jsonOut += f'\n      "Parent": "{bone.parent.name}",'
        mtx = bone.parent.matrix_local.inverted() @ mtx
    jsonOut += '\n      "Translation": [' + ','.join(str(val) for val in mtx.to_translation()) + "],"
    jsonOut += '\n      "Rotation": [' + ','.join(str(val) for val in mtx.to_euler("XYZ")) + "],"
    jsonOut += '\n      "Scale": [' + ','.join(str(val) for val in mtx.to_scale()) + "],"
    # Footer
    jsonOut = jsonOut.rstrip(',')
    jsonOut += '\n    },'
    return jsonOut

def ExportArmature(armature:bpy.types.Armature, name:str):
    ""
    # Header
    jsonOut = '{\n  "Armature": {'
    # Data
    jsonOut += '\n    "Name": "' + name + '",'
    # Bones
    jsonOut += '\n    "Bones": ['
    for bone in armature.bones:
        jsonOut += ExportBone(bone)
    jsonOut = jsonOut.rstrip(',')
    jsonOut += '\n    ],'
    # Footer
    jsonOut = jsonOut.rstrip(',')
    jsonOut += '\n  }\n}'
    return jsonOut

EXPORT_CONSTANT_CURVE = False
EXPORT_DUR = 0

def ExportCurve(curve:bpy.types.FCurve,curveIsConst:bool,curveConst:float):
    ""
    global EXPORT_DUR
    # Header
    jsonOut = '\n    {'
    # Data
    kfs = curve.keyframe_points
    if curveIsConst:
        jsonOut += f'\n      "Type": "Constant",'
        jsonOut += f'\n      "Constant": {curveConst},'
    else:
        # Check for the most complex keyframe in the curve, this defines the complexity of the exported curve.
        curveTypeName = "Invalid"
        curveType = -1
        CURVETYPE_STEPPED = 0
        CURVETYPE_LINEAR = 1
        CURVETYPE_CUBIC = 2
        for kf in kfs:
            if kf.interpolation == "CONSTANT" and curveType < CURVETYPE_STEPPED:
                curveType = CURVETYPE_STEPPED
                curveTypeName = "Stepped"
            elif kf.interpolation == "LINEAR" and curveType < CURVETYPE_LINEAR:
                curveType = CURVETYPE_LINEAR
                curveTypeName = "Linear"
            elif kf.interpolation == "BEZIER" and curveType < CURVETYPE_CUBIC:
                curveType = CURVETYPE_CUBIC
                curveTypeName = "Cubic"
            EXPORT_DUR = max(kf.co.x * _FPS_FACTOR, EXPORT_DUR)
        jsonOut += f'\n      "Type": "{curveTypeName}",'
        jsonOut += f'\n      "Data": ['
        match curveType:
            case 0: # CURVETYPE_STEPPED
                for i, kf in enumerate(kfs):
                    co = kf.co.copy()
                    co.x *= _FPS_FACTOR
                    jsonOut += "\n        " + ''.join(f'{val}, ' for val in co)
            case 1: # CURVETYPE_LINEAR
                for i, kf in enumerate(kfs):
                    co = kf.co.copy()
                    co.x *= _FPS_FACTOR
                    if kf.interpolation == "LINEAR":
                        jsonOut += "\n        " + ''.join(f'{val}, ' for val in co)
                    elif kf.interpolation == "CONSTANT":
                        jsonOut += "\n        " + ''.join(f'{val}, ' for val in kf.co)
                        if i < len(kfs) - 1:
                            nextCo = co.copy()
                            nextCo.x -= kfs[i+1].co.x * _FPS_FACTOR - 0.0001
                            jsonOut += "\n        " + ''.join(f'{val}, ' for val in nextCo)
            case 2: # CURVETYPE_CUBIC
                for i, kf in enumerate(kfs):
                    co = kf.co.copy()
                    co.x *= _FPS_FACTOR
                    vals = list(co) + [0] * 2
                    if kf.interpolation == "BEZIER":
                        if i > 0:
                            diff = kf.handle_right - kf.co
                            tanOut = diff.y/diff.x
                            vals[2] = tanOut
                        if i < len(kfs) - 1:
                            diff = kfs[i+1].co - kfs[i+1].handle_left
                            tanIn = diff.y/diff.x
                            vals[3] = tanIn
                        jsonOut += "\n        " + ''.join(f'{val}, ' for val in vals)
                    elif kf.interpolation == "LINEAR":
                        if i < len(kfs) - 1:
                            nextCo = kfs[i+1].co.copy()
                            diff = nextCo - co
                            tan = diff.y/diff.x
                            vals[2] = tan
                            vals[3] = tan
                        jsonOut += "\n        " + ''.join(f'{val}, ' for val in vals)
                    elif kf.interpolation == "CONSTANT":
                        jsonOut += "\n        " + ''.join(f'{val}, ' for val in vals)
                        if i < len(kfs) - 1:
                            nextCo = co.copy()
                            nextCo.x -= kfs[i+1].co.x * _FPS_FACTOR - 0.0001
                            valsNext = list(nextCo) + [0] * 2
                            jsonOut += "\n        " + ''.join(f'{val}, ' for val in valsNext)
        jsonOut = jsonOut.rstrip(',')
        jsonOut += '\n      ],'
    # Footer
    jsonOut = jsonOut.rstrip(',')
    jsonOut += '\n    }'
    return jsonOut

def ExportCurves(action:bpy.types.Action):
    ""
    global EXPORT_CONSTANT_CURVE
    curveMap:dict[bpy.types.FCurve,int|float]=dict()
    jsonCurves:list[str]=list()
    for curve in action.fcurves:
        kfs = curve.keyframe_points
        curveIsConst = len(kfs) <= 1
        curveConst = 0.0 if len(kfs) < 1 else kfs[0].co.y
        if not curveIsConst:
            EPSILON = 0.00001
            curveConst = kfs[0].co.y
            for kf in kfs[1:]:
                if abs(kf.co.y - curveConst) > EPSILON or abs(kf.co.y - kf.handle_left.y) > EPSILON or abs(kf.co.y - kf.handle_right.y) > EPSILON:
                    break
            else:
                curveIsConst = True
        if EXPORT_CONSTANT_CURVE and curveIsConst:
            curveMap[curve] = curveConst
            continue
        if curveConst == 0.0:
            curveConst = 0.0
        jsonCurve = ExportCurve(curve, curveIsConst, curveConst)
        jsonCurveIndex = jsonCurves.index(jsonCurve) if jsonCurve in jsonCurves else -1
        if jsonCurveIndex < 0:
            jsonCurveIndex = len(jsonCurves)
            jsonCurves.append(jsonCurve)
        curveMap[curve] = jsonCurveIndex
    return ",".join(jsonCurves), curveMap

def ExportCurveGroup(groups:bpy.types.ActionGroups, jsonMap:dict[bpy.types.FCurve,int|float]):
    ""
    jsonOut = ""
    modelLink:list[str] = list()
    modelCurves:list[float|int] = list()
    for group in groups:
        if all(channel.data_path.startswith("pose.bones") for channel in group.channels):
            # This is a model link
            fcu = group.channels[0]
            boneName = fcu.data_path.partition('[')[2][1:].rpartition(']')[0][:-1]
            translation = [None]*3
            rotation = [None]*3
            scale = [None]*3
            for fcu in group.channels:
                if fcu.data_path.endswith('location'):
                    translation[fcu.array_index] = jsonMap[fcu]
                elif fcu.data_path.endswith('rotation_euler'):
                    rotation[fcu.array_index] = jsonMap[fcu]
                elif fcu.data_path.endswith('scale'):
                    scale[fcu.array_index] = jsonMap[fcu]
            translationExists = None not in translation
            rotationExists = None not in rotation
            scaleExists = None not in scale
            translationUniform = translation[0] == translation[1] == translation[2] and (isinstance(translation[0], float) and isinstance(translation[1], float) and isinstance(translation[2], float) or isinstance(translation[0], int) and isinstance(translation[1], int) and isinstance(translation[2], int))
            rotationUniform = rotation[0] == rotation[1] == rotation[2] and (isinstance(rotation[0], float) and isinstance(rotation[1], float) and isinstance(rotation[2], float) or isinstance(rotation[0], int) and isinstance(rotation[1], int) and isinstance(rotation[2], int))
            scaleUniform = scale[0] == scale[1] == scale[2] and (isinstance(scale[0], float) and isinstance(scale[1], float) and isinstance(scale[2], float) or isinstance(scale[0], int) and isinstance(scale[1], int) and isinstance(scale[2], int))
            # Header
            boneOut = '\n      {'
            # Data
            boneOut += f'\n        "Name": "{boneName}",'
            boneOut += f'\n        "Flags": ['
            if scaleExists:
                boneOut += f'\n          "ScaleExists",'
                if scaleUniform:
                    boneOut += f'\n          "ScaleUniform",'
                if isinstance(scale[0], float):
                    boneOut += f'\n          "ScaleXConst",'
                if isinstance(scale[1], float):
                    boneOut += f'\n          "ScaleYConst",'
                if isinstance(scale[2], float):
                    boneOut += f'\n          "ScaleZConst",'
            if rotationExists:
                boneOut += f'\n          "RotationExists",'
                if rotationUniform:
                    boneOut += f'\n          "RotationUniform",'
                if isinstance(rotation[0], float):
                    boneOut += f'\n          "RotationXConst",'
                if isinstance(rotation[1], float):
                    boneOut += f'\n          "RotationYConst",'
                if isinstance(rotation[2], float):
                    boneOut += f'\n          "RotationZConst",'
            if translationExists:
                boneOut += f'\n          "TranslationExists",'
                if translationUniform:
                    boneOut += f'\n          "TranslationUniform",'
                if isinstance(translation[0], float):
                    boneOut += f'\n          "TranslationXConst",'
                if isinstance(translation[1], float):
                    boneOut += f'\n          "TranslationYConst",'
                if isinstance(translation[2], float):
                    boneOut += f'\n          "TranslationZConst",'
            # Bone Flags
            boneOut = boneOut.rstrip(',')
            boneOut += '\n        ],'
            if scaleExists:
                if scaleUniform:
                    modelCurves.append(scale[0])
                else:
                    modelCurves += scale
            if rotationExists:
                if rotationUniform:
                    modelCurves.append(rotation[0])
                else:
                    modelCurves += rotation
            if translationExists:
                if translationUniform:
                    modelCurves.append(translation[0])
                else:
                    modelCurves += translation
            # Footer
            boneOut = boneOut.rstrip(',')
            boneOut += '\n      },'
            modelLink.append(boneOut)
            continue
    # Model Finish
    if modelLink and modelCurves:
        # Header
        jsonOut += '\n    {'
        # Data
        jsonOut += f'\n      "Type": "Model",'
        jsonOut += f'\n      "Bones": ['
        jsonOut += '\n      '.join(modelLink)
        jsonOut = jsonOut.rstrip(',')
        jsonOut += f'\n      ],'
        jsonOut += f'\n      "Curves": ['
        jsonOut += f'\n        ' + ', '.join(str(val) for val in modelCurves)
        jsonOut += f'\n      ],'
        # Footer
        jsonOut = jsonOut.rstrip(',')
        jsonOut += '\n    },'
    return jsonOut

def ExportAction(action:bpy.types.Action, name:str):
    ""
    global EXPORT_DUR
    EXPORT_DUR = 0
    # Header
    jsonOut = '{\n  "Animation": {'
    # Data
    jsonOut += '\n    "Name": "' + name + '",'
    # Curves
    jsonOut += '\n    "Curves": ['
    curveOut, curveMap = ExportCurves(action)
    jsonOut += curveOut
    jsonOut = jsonOut.rstrip(',')
    jsonOut += '\n    ],'
    # Links
    jsonOut += '\n    "Links": ['
    linkOut = ExportCurveGroup(action.groups, curveMap)
    jsonOut += linkOut
    jsonOut = jsonOut.rstrip(',')
    jsonOut += '\n    ],'
    jsonOut += f'\n    "Duration": {EXPORT_DUR},'
    loopMode = "Mirror"
    jsonOut += f'\n    "Loop": "{loopMode}",'
    # Footer
    jsonOut = jsonOut.rstrip(',')
    jsonOut += '\n  }\n}'
    return jsonOut

class NightjarExportMesh(bpy.types.Operator):
    from bpy.props import StringProperty, BoolProperty, CollectionProperty
    """Exporter for Nightjar meshes"""
    bl_idname = "export.nightjar_mesh"
    bl_label = "Export Nightjar Mesh (.json)"

    filepath: StringProperty(name="File Path", description="", subtype="FILE_PATH", maxlen=1024, default="", options={'HIDDEN'})
    filter_glob: StringProperty(default='*.json', options={'HIDDEN'}, maxlen=255)
    filename: StringProperty(subtype='FILE_NAME', default='', options={'HIDDEN'})
    filename_ext = ".json"

    @classmethod
    def poll(cls, context):
        return context.object is not None and context.object.type == "MESH"

    def execute(self, context):
        obj = context.object
        jsonOut = ExportMesh(obj.data, obj.name)
        print(jsonOut)
        with open(self.filepath, "wt") as f:
            f.write(jsonOut)
        return {'FINISHED'}

    def invoke(self, context, event):
        self.filename = bpy.path.ensure_ext(context.object.name, self.filename_ext)
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}
    
    @staticmethod
    def menu_func(self, context):
        self.layout.operator_context = 'INVOKE_DEFAULT'
        self.layout.operator(NightjarExportMesh.bl_idname, text="Export Nightjar Mesh (.json)")

class NightjarExportArmature(bpy.types.Operator):
    from bpy.props import StringProperty, BoolProperty, CollectionProperty
    """Exporter for Nightjar armatures"""
    bl_idname = "export.nightjar_armature"
    bl_label = "Export Nightjar Armature (.json)"

    filepath: StringProperty(name="File Path", description="", subtype="FILE_PATH", maxlen=1024, default="", options={'HIDDEN'})
    filter_glob: StringProperty(default='*.json', options={'HIDDEN'}, maxlen=255)
    filename: StringProperty(subtype='FILE_NAME', default='', options={'HIDDEN'})
    filename_ext = ".json"

    @classmethod
    def poll(cls, context):
        return context.object is not None and context.object.type == "ARMATURE"

    def execute(self, context):
        obj = context.object
        jsonOut = ExportArmature(obj.data, obj.name)
        print(jsonOut)
        with open(self.filepath, "wt") as f:
            f.write(jsonOut)
        return {'FINISHED'}

    def invoke(self, context, event):
        self.filename = bpy.path.ensure_ext(context.object.name, self.filename_ext)
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

    @staticmethod
    def menu_func(self, context):
        self.layout.operator_context = 'INVOKE_DEFAULT'
        self.layout.operator(NightjarExportArmature.bl_idname, text="Export Nightjar Armature (.json)")

class NightjarExportAction(bpy.types.Operator):
    from bpy.props import StringProperty, BoolProperty, CollectionProperty
    """Exporter for Nightjar animations"""
    bl_idname = "export.nightjar_action"
    bl_label = "Export Nightjar Animation (.json)"

    filepath: StringProperty(name="File Path", description="", subtype="FILE_PATH", maxlen=1024, default="", options={'HIDDEN'})
    filter_glob: StringProperty(default='*.json', options={'HIDDEN'}, maxlen=255)
    filename: StringProperty(subtype='FILE_NAME', default='', options={'HIDDEN'})
    filename_ext = ".json"

    @classmethod
    def poll(cls, context):
        obj = context.object
        return obj is not None and obj.animation_data is not None and obj.animation_data.action is not None

    def execute(self, context):
        global _FPS_FACTOR
        obj = context.object
        scn = context.scene
        _FPS_FACTOR = scn.render.fps_base / scn.render.fps
        jsonOut = ExportAction(obj.animation_data.action, obj.animation_data.action.name)
        print(jsonOut)
        with open(self.filepath, "wt") as f:
            f.write(jsonOut)
        return {'FINISHED'}

    def invoke(self, context, event):
        self.filename = bpy.path.ensure_ext(context.object.animation_data.action.name, self.filename_ext)
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

    @staticmethod
    def menu_func(self, context):
        self.layout.operator_context = 'INVOKE_DEFAULT'
        self.layout.operator(NightjarExportAction.bl_idname, text="Export Nightjar Animation (.json)")

if __name__ == "__main__":
    print('-'*20)
    # Operator search based on google AI result from prompt "blender python unregister operator by name". Result did not cite sources.
    operator_idname = { NightjarExportMesh.bl_idname, NightjarExportArmature.bl_idname, NightjarExportAction.bl_idname, }
    for cls in bpy.types.Operator.__subclasses__():
        if hasattr(cls, 'bl_idname') and cls.bl_idname == operator_idname:
            try:
                bpy.utils.unregister_class(cls)
                # print(f"Operator '{operator_idname}' unregistered successfully.")
            except RuntimeError as e:
                pass
            try:
                bpy.types.TOPBAR_MT_file_export.remove(cls.menu_func)
                # print(f"Button '{operator_idname}' removed successfully.")
            except RuntimeError as e:
                pass
    bpy.utils.register_class(NightjarExportMesh)
    bpy.types.TOPBAR_MT_file_export.append(NightjarExportMesh.menu_func)
    bpy.utils.register_class(NightjarExportArmature)
    bpy.types.TOPBAR_MT_file_export.append(NightjarExportArmature.menu_func)
    bpy.utils.register_class(NightjarExportAction)
    bpy.types.TOPBAR_MT_file_export.append(NightjarExportAction.menu_func)
