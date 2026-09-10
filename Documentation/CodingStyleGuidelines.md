
Headers

	Categories of headers:
		static instance class
			systems, libraries, etc
			- all fields shall be declared as private members of the class.
			- all public functions shall be declared as static.
			- the constructor, destructor, and instance variable shall all be private and inaccessible (no getter).
		instance classes
			containers, objects(components)/namedobjects(entities, meshes), etc
	- All functions to be declared in Pascal case (PascalCase).
	- No public class fields
		- If you need to make it "public," make a simple getter/setter
	- Getter/Setter function names are to be declared as Get<Name> and Set<Name>, where <Name> is the name of the field.
	- Use references in place of pointers as much as possible (there are few cases where you need a pointer instead of a reference).
	- As much as possible, use typedefs instead of includes to reduce build times due to chained includes.
	- Enum value names are to begin with "c<Name>" (ie "cTransform").
	- All code shall be encompassed within the Nightjar namespace.
	
	Templated functions:
		- Declare them in the appropriate location, then implement them in a file named <Name>Template.cpp, and include that file at the end of the header as "#include "<Name>Template.cpp"", where <Name> is the name of the header file.

Implementation
	- Curly brackets are to be placed on the following line, unless they are being used as a constructor call (i.e. "NamedObject obj{"Example"};")
	- if statements may omit curly brackets only if there is one line of code following it.
	- Indentation uses steps of 4 spaces.
	- Indentation must be used within curly brackets and multi-line statements.
	- No global variables!
	- Initializer lists are to be spread out on lines as so:
		Example::Example()
			: NamedObject()
			, Field1()
			, Field2()
		{
		}
		On top of consistency, this works smoothly with git's diff when pushing and pulling.
