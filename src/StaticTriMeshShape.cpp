/**	@file	StaticTriMeshShape.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Physics::StaticTriMeshShape'
*/

#include <Athena-Physics/StaticTriMeshShape.h>
#include <Athena-Physics/Body.h>
#include <Athena-Physics/Conversions.h>

using namespace Athena;
using namespace Athena::Physics;
using namespace Athena::Entities;
using namespace Athena::Utils;
using namespace Athena::Math;
using namespace std;


/************************************** CONSTANTS **************************************/

///< Name of the type of component
const std::string StaticTriMeshShape::TYPE = "Athena/Physics/StaticTriMeshShape";


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

StaticTriMeshShape::StaticTriMeshShape(const std::string& strName, ComponentsList* pList)
: CollisionShape(strName, pList), m_sourceType(SOURCE_INDEXED_MESHES), m_indexedStrider(0)
{
    m_pCollisionShape = new btBvhTriangleMeshShape();
}

//-----------------------------------------------------------------------

StaticTriMeshShape::~StaticTriMeshShape()
{
    if (m_sourceType == SOURCE_INDEXED_MESHES)
    {
        delete m_indexedStrider;
        
        for (unsigned int i = 0; i < m_ownedIndexedMeshes.size(); ++i)
        {
            delete[] m_ownedIndexedMeshes[i].m_triangleIndexBase;
            delete[] m_ownedIndexedMeshes[i].m_vertexBase;
        }
    }
}

//-----------------------------------------------------------------------

StaticTriMeshShape* StaticTriMeshShape::create(const std::string& strName, ComponentsList* pList)
{
	return new StaticTriMeshShape(strName, pList);
}

//-----------------------------------------------------------------------

StaticTriMeshShape* StaticTriMeshShape::cast(Component* pComponent)
{
	return dynamic_cast<StaticTriMeshShape*>(pComponent);
}


/*********************************** METHODS **********************************/

bool StaticTriMeshShape::addMesh(unsigned int nbVertices, const float* pVertices,
                                 unsigned int nbIndices, const int* pIndices,
                                 tMemoryManagment memManagment)
{
    // Assertions
    assert(nbVertices > 0);
    assert(pVertices);
    assert(nbIndices > 0);
    assert(pIndices);

    if (m_sourceType != SOURCE_INDEXED_MESHES)
        return false;
    
    if (!m_indexedStrider)
        m_indexedStrider = new btTriangleIndexVertexArray();

    btIndexedMesh mesh;
    mesh.m_vertexType           = PHY_FLOAT;
    mesh.m_numTriangles         = nbIndices / 3;
    mesh.m_triangleIndexStride  = 3 * sizeof(int);
    mesh.m_numVertices          = nbVertices;
    mesh.m_vertexStride         = 3 * sizeof(float);

    if (memManagment == MEM_COPY)
    {
        mesh.m_triangleIndexBase = new unsigned char[nbIndices * 3 * sizeof(int)];
        memcpy((void*) mesh.m_triangleIndexBase, pIndices, nbIndices * 3 * sizeof(int));
        
        mesh.m_vertexBase = new unsigned char[nbVertices * 3 * sizeof(float)];
        memcpy((void*) mesh.m_vertexBase, pVertices, nbVertices * 3 * sizeof(float));
    }
    else
    {
        mesh.m_triangleIndexBase    = (const unsigned char*) pIndices;
        mesh.m_vertexBase           = (const unsigned char*) pVertices;
    }

    if (memManagment != MEM_REFERENCE)
        m_ownedIndexedMeshes.push_back(mesh);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    m_indexedStrider->addIndexedMesh(mesh);

    delete m_pCollisionShape;
    m_pCollisionShape = new btBvhTriangleMeshShape(m_indexedStrider, true);

    if (pBody)
        pBody->setCollisionShape(this);
    
    return true;
}

//-----------------------------------------------------------------------

bool StaticTriMeshShape::addMesh(unsigned int nbVertices, const double* pVertices,
                                 unsigned int nbIndices, const int* pIndices,
                                 tMemoryManagment memManagment)
{
    // Assertions
    assert(nbVertices > 0);
    assert(pVertices);
    assert(nbIndices > 0);
    assert(pIndices);

    if (m_sourceType != SOURCE_INDEXED_MESHES)
        return false;
    
    if (!m_indexedStrider)
        m_indexedStrider = new btTriangleIndexVertexArray();

    btIndexedMesh mesh;
    mesh.m_vertexType           = PHY_DOUBLE;
    mesh.m_numTriangles         = nbIndices / 3;
    mesh.m_triangleIndexStride  = 3 * sizeof(int);
    mesh.m_numVertices          = nbVertices;
    mesh.m_vertexStride         = 3 * sizeof(double);

    if (memManagment == MEM_COPY)
    {
        mesh.m_triangleIndexBase = new unsigned char[nbIndices * 3 * sizeof(int)];
        memcpy((void*) mesh.m_triangleIndexBase, pIndices, nbIndices * 3 * sizeof(int));
        
        mesh.m_vertexBase = new unsigned char[nbVertices * 3 * sizeof(double)];
        memcpy((void*) mesh.m_vertexBase, pVertices, nbVertices * 3 * sizeof(double));
    }
    else
    {
        mesh.m_triangleIndexBase    = (const unsigned char*) pIndices;
        mesh.m_vertexBase           = (const unsigned char*) pVertices;
    }

    if (memManagment != MEM_REFERENCE)
        m_ownedIndexedMeshes.push_back(mesh);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    m_indexedStrider->addIndexedMesh(mesh);

    delete m_pCollisionShape;
    m_pCollisionShape = new btBvhTriangleMeshShape(m_indexedStrider, true);

    if (pBody)
        pBody->setCollisionShape(this);
    
    return true;
}

//-----------------------------------------------------------------------

bool StaticTriMeshShape::addMesh(unsigned int nbVertices, const Math::Vector3* pVertices,
                                 unsigned int nbIndices, const int* pIndices,
                                 tMemoryManagment memManagment)
{
    // Assertions
    assert(nbVertices > 0);
    assert(pVertices);
    assert(nbIndices > 0);
    assert(pIndices);

    if (m_sourceType != SOURCE_INDEXED_MESHES)
        return false;

    if (memManagment == MEM_REFERENCE)
        memManagment = MEM_COPY;
    
    if (!m_indexedStrider)
        m_indexedStrider = new btTriangleIndexVertexArray();

    btIndexedMesh mesh;
    mesh.m_vertexType           = PHY_FLOAT;
    mesh.m_numTriangles         = nbIndices / 3;
    mesh.m_triangleIndexStride  = 3 * sizeof(int);
    mesh.m_numVertices          = nbVertices;
    mesh.m_vertexStride         = 3 * sizeof(float);
    mesh.m_vertexBase           = new unsigned char[nbVertices * 3 * sizeof(float)];

    Vector3*    pSrc = (Vector3*) pVertices;
    float*      pDst = (float*) mesh.m_vertexBase;
    for (unsigned int i = 0; i < nbVertices; ++i)
    {
        pDst[0] = (*pSrc).x;
        pDst[1] = (*pSrc).y;
        pDst[2] = (*pSrc).z;
        
        ++pSrc;
        pDst += 3;
    }

    if (memManagment == MEM_COPY)
    {
        mesh.m_triangleIndexBase = new unsigned char[nbIndices * 3 * sizeof(int)];
        memcpy((void*) mesh.m_triangleIndexBase, pIndices, nbIndices * 3 * sizeof(int));
    }
    else
    {
        mesh.m_triangleIndexBase = (const unsigned char*) pIndices;
    
        delete[] pVertices;
    }

    m_ownedIndexedMeshes.push_back(mesh);

    Body* pBody = m_pBody;
    if (pBody)
        pBody->setCollisionShape(0);

    m_indexedStrider->addIndexedMesh(mesh);

    delete m_pCollisionShape;
    m_pCollisionShape = new btBvhTriangleMeshShape(m_indexedStrider, true);

    if (pBody)
        pBody->setCollisionShape(this);
    
    return true;
}


/***************************** MANAGEMENT OF THE PROPERTIES ****************************/

Utils::PropertiesList* StaticTriMeshShape::getProperties() const
{
	// Call the base class implementation
	PropertiesList* pProperties = Component::getProperties();

	// Create the category belonging to this type
	pProperties->selectCategory(TYPE, false);

	// Returns the list
	return pProperties;
}

//-----------------------------------------------------------------------

bool StaticTriMeshShape::setProperty(const std::string& strCategory, const std::string& strName,
								Utils::Variant* pValue)
{
	assert(!strCategory.empty());
	assert(!strName.empty());
	assert(pValue);

	if (strCategory == TYPE)
		return StaticTriMeshShape::setProperty(strName, pValue);

	return Component::setProperty(strCategory, strName, pValue);
}

//-----------------------------------------------------------------------

bool StaticTriMeshShape::setProperty(const std::string& strName, Utils::Variant* pValue)
{
	// Assertions
	assert(!strName.empty());
	assert(pValue);

	// Destroy the value
	delete pValue;

	return true;
}
