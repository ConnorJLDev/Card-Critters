//SCENE STUFF
// Entity E (ally bird)
entity = new Entity("Entity E");
transform = (Transform)transform->Clone();
transform->SetTranslation(&posE);
transform->SetRotation(0.0f);
transform->SetScale(&scaleC);
entity->ComponentAdd(transform);
physics = (Physics)physics->Clone();
physics->SetRotationalVelocity(0.0f);
entity->ComponentAdd(physics);
sprite = sprite->Clone();
sprite->SetSpriteSource("AllyBird");
entity->ComponentAdd(sprite);
BehaviorHealth* allyHealth = new BehaviorHealth(3);
entity->ComponentAdd(allyHealth);
Behavior* allyBehavior = new BehaviorEnemy();
entity->ComponentAdd(allyBehavior);
entities->Add(entity);
//Entity D
entity = new Entity("Entity D");
transform = (Transform)transform->Clone();
transform->SetTranslation(&posD);
transform->SetRotation(0.0f);
transform->SetScale(&scaleD);
entity->ComponentAdd(transform);
physics = (Physics)physics->Clone();
physics->SetRotationalVelocity(0.0f);
entity->ComponentAdd(physics);
sprite = sprite->Clone();
sprite->SetSpriteSource("TestCard");
entity->ComponentAdd(sprite);
ColliderRect Rcollider = new ColliderRect();
Rcollider->setWidth(collD.x);
Rcollider->setLength(collD.y);
entity->ComponentAdd(Rcollider);

Behavior cardBehavior = new BehaviorCard();
entity->ComponentAdd(cardBehavior);
entities->Add(entity);
// Entity B
entity = new Entity("Entity B");
transform = (Transform)transform->Clone();
transform->SetTranslation(&posB);
transform->SetRotation(45.0f);
entity->ComponentAdd(transform);
physics = (Physics)physics->Clone();
entity->ComponentAdd(physics);
sprite = sprite->Clone();
sprite->SetSpriteSource("Ball");
entity->ComponentAdd(sprite);
entities->Add(entity);
//////////////////////////////////////////

//MISC STUFF
// Tell the compiler that the 'dt' variable is unused.
        //UNREFERENCED_PARAMETER(dt);

entities->UpdateAll(dt);
entities->CheckCollisions();
// NOTE: This call causes the engine to exit immediately.
        //   Make sure to remove it when you are ready to test out a new scene.
        //SceneSystem::SetNext(nullptr);