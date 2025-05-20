# BulletECS

Bullet ECS is a modern C++ library that adds a layer on top of the [Bullet Physics Library](https://github.com/Bulletphysics/Bullet3)
managing its objects through an Entity Component System architecture, improving the Data Locality.

This library is tightly coupled to Bullet, but apart from using its LinearMath structs like `btVector3`,
it does not require the user to directly interact with Bullet, instead it provides wrappers to its 
functionality so it is easier to use.

This library covers a subset of the Bullet's functionality, but follows the Open-Close principle by 
allowing the user to directly interact with the Bullet API
if a functionality not covered by this library is needed.

# Implementation Details

Each Bullet physics object, like btRigidBody or btBoxShape, is referenced by the Bullet Dynamics World 
using pointers. That means once created, a Bullet physics object cannot be moved from its location without 
leaving a dangling pointer in the btDynamicsWorld. That means a sparse set cannot be used to store 
these objects, now being treated as components by the
ECS, because the sparse set moves the objects in the dense array so they are always tightly packed.
So for an entity's whole lifetime, its components (collision shape and rigidbody) will be at the same memory location.


