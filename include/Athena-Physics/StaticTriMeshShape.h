/** @file	StaticTriMeshShape.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Physics::StaticTriMeshShape'
*/

#ifndef _ATHENA_PHYSICS_STATICTRIMESHSHAPE_H_
#define _ATHENA_PHYSICS_STATICTRIMESHSHAPE_H_

#include <Athena-Physics/Prerequisites.h>
#include <Athena-Physics/CollisionShape.h>

namespace Athena {
namespace Physics {


//---------------------------------------------------------------------------------------
/// @brief	Static triangle mesh shape
///
/// The geometry of the triangle mesh can be specified either:
///   - by providing some lists of indices and vertices (if multiple lists are provided,
///     they are combined to form the triangle mesh shape)
///   - if support for Athena-Graphics is enabled: as one Ogre mesh or one Visual Object
///     component
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL StaticTriMeshShape: public CollisionShape
{
    //_____ Internal types ___________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	The memory management modes for the arrays provided by the application
    //-----------------------------------------------------------------------------------
    enum tMemoryManagment
    {
        MEM_REFERENCE,      ///< Keep only a reference to the arrays, the application is
                            ///< responsible to keep them around and to delete them when
                            ///< not needed anymore
        MEM_COPY,           ///< The trimesh shape must make a copy of the arrays
        MEM_TAKE_OWNERSHIP, ///< The trimesh shape is responsible to free the memory of
                            ///< the arrays
    };


	//_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	Constructor
    /// @param	strName		Name of the component
    //-----------------------------------------------------------------------------------
	StaticTriMeshShape(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Create a new component (Component creation method)
    ///
    /// @param	strName	Name of the component
    /// @param	pList	List to which the component must be added
    /// @return			The new component
    //-----------------------------------------------------------------------------------
	static StaticTriMeshShape* create(const std::string& strName, Entities::ComponentsList* pList);

    //-----------------------------------------------------------------------------------
    /// @brief	Cast a component to a StaticTriMeshShape
    ///
    /// @param	pComponent	The component
    /// @return				The component, 0 if it isn't castable to a StaticTriMeshShape
    //-----------------------------------------------------------------------------------
	static StaticTriMeshShape* cast(Entities::Component* pComponent);

protected:
    //-----------------------------------------------------------------------------------
    /// @brief	Destructor
    //-----------------------------------------------------------------------------------
	virtual ~StaticTriMeshShape();


	//_____ Implementation of Component __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Returns the type of the component
	/// @return	The type
	//-----------------------------------------------------------------------------------
	virtual const std::string getType() const { return TYPE; }


	//_____ Methods __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Add a mesh to the trimesh shape
	///
	/// @param  nbVertices	    The number of vertices
	/// @param  pVertices	    The array of vertices
	/// @param  nbIndices	    The number of indices
	/// @param  pIndices	    The array of indices
	/// @param  memManagment	Memory managment mode for the arrays
	//-----------------------------------------------------------------------------------
	bool addMesh(unsigned int nbVertices, const float* pVertices,
	             unsigned int nbIndices, const int* pIndices,
	             tMemoryManagment memManagment = MEM_REFERENCE);

	//-----------------------------------------------------------------------------------
	/// @brief	Add a mesh to the trimesh shape
	///
	/// @param  nbVertices	    The number of vertices
	/// @param  pVertices	    The array of vertices
	/// @param  nbIndices	    The number of indices
	/// @param  pIndices	    The array of indices
	/// @param  memManagment	Memory managment mode for the arrays
	//-----------------------------------------------------------------------------------
 	bool addMesh(unsigned int nbVertices, const double* pVertices,
 	             unsigned int nbIndices, const int* pIndices,
 	             tMemoryManagment memManagment = MEM_REFERENCE);

	//-----------------------------------------------------------------------------------
	/// @brief	Add a mesh to the trimesh shape
	///
	/// @param  nbVertices	    The number of vertices
	/// @param  pVertices	    The array of vertices
	/// @param  nbIndices	    The number of indices
	/// @param  pIndices	    The array of indices
	/// @param  memManagment	Memory managment mode for the arrays (only MEM_COPY or
	///                         MEM_TAKE_OWNERSHIP are accepted)
	///
	/// @remark If the trimesh shape must takes ownership of the arrays, ensure that it
	///         has the right to delete[] pVertices!
	//-----------------------------------------------------------------------------------
	bool addMesh(unsigned int nbVertices, const Math::Vector3* pVertices,
			     unsigned int nbIndices, const int* pIndices,
  	             tMemoryManagment memManagment = MEM_COPY);

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the Bullet's static triangle mesh shape
	//-----------------------------------------------------------------------------------
    inline btBvhTriangleMeshShape* getStaticTriMeshShape() const
    {
        return (btBvhTriangleMeshShape*) m_pCollisionShape;
    }
    

	//_____ Management of the properties __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	Returns a list containing the properties of the component
    ///
    /// Used in the serialization mecanism of the components
    /// @remark	Must be overriden by each component type. Each implementation must first call
    ///			its base class one, and add a new category (named after the component's type)
    ///			AT THE BEGINNING of the obtained list, containing the properties related to
    ///			this type.
    /// @return	The list of properties
    //-----------------------------------------------------------------------------------
	virtual Utils::PropertiesList* getProperties() const;
	
    //-----------------------------------------------------------------------------------
    /// @brief	Set the value of a property of the component
    ///
    /// Used in the deserialization mecanism of the parts
    ///	@param	strCategory		The category of the property
    ///	@param	strName			The name of the property
    ///	@param	pValue			The value of the property
    ///	@return					'true' if the property was used, 'false' if a required object
    ///							is missing
    /// @remark	Must be overriden by each component type. Each implementation must test if the
    ///			property's category is the one of the component's type, and if so process the
    ///			property's value. Otherwise, it must call its base class implementation.
    //-----------------------------------------------------------------------------------
	virtual bool setProperty(const std::string& strCategory, const std::string& strName,
							 Utils::Variant* pValue);

    //-----------------------------------------------------------------------------------
    /// @brief	Set the value of a property of the component
    ///
    /// Used in the deserialization mecanism of the parts
    ///	@param	strName		The name of the property
    ///	@param	pValue		The value of the property
    ///	@return				'true' if the property was used, 'false' if a required object
    ///						is missing
    //-----------------------------------------------------------------------------------
	bool setProperty(const std::string& strName, Utils::Variant* pValue);


    //_____ Internal types ___________
protected:
    enum tSourceType
    {
        SOURCE_INDEXED_MESHES,
#if ATHENA_PHYSICS_GRAPHICS_SUPPORT
        SOURCE_OGRE_MESH,
#endif
    };


	//_____ Constants __________
public:
	static const std::string TYPE;	///< Name of the type of component


    //_____ Attributes __________
protected:
    tSourceType         m_sourceType;           ///< Type of data source
    IndexedMeshArray    m_ownedIndexedMeshes;   ///< Indexed meshes that are owned by the trimesh shape

#if ATHENA_PHYSICS_GRAPHICS_SUPPORT
    union
    {
        btTriangleIndexVertexArray* m_indexedStrider;
        OgreMeshStrider*            m_ogreStrider;
    };
#else
    btTriangleIndexVertexArray* m_indexedStrider;
#endif
};

}
}

#endif
