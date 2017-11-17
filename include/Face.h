#ifndef __KKC_FACE__
#define __KKC_FACE__

#include "ArraySimple.h"

//
// Face is a small helper class used by the Advancing Front mesh generator.
// Its purpose is to make the bookkeeping of newly generated faces and elements
// a little easier to perform and read.
// 

class Face 
{
public:
  //inline Face(const IntegerArray &vertices_, int z1_, int z2_, int id_)
  inline Face(const ArraySimple<int> &vertices_, int z1_, int z2_, int id_)
  {
    vertices[0] = vertices[1] = vertices[2] = vertices[3] = -1;

    nVerts = vertices_.size(0);

    // should check to make sure nVerts<=4 ?
    for ( int v=0; v<nVerts; v++ )
      vertices[v] = vertices_[v];

    z1 = z1_;
    z2 = z2_;
    id = id_;
  }

  inline Face(const ArraySimpleFixed<int,3,1,1,1> &vertices_, int z1_, int z2_, int id_)
  {
    vertices[0] = vertices[1] = vertices[2] = vertices[3] = -1;

    nVerts = 3;

    // should check to make sure nVerts<=4 ?
    for ( int v=0; v<nVerts; v++ )
      vertices[v] = vertices_[v];

    z1 = z1_;
    z2 = z2_;
    id = id_;
  }

  inline virtual ~Face() { }
  inline int getID() const { return id; }
  inline int getVertex(const int &v) const { return vertices[v]; }
  inline int getZ1ID() const { return z1; }
  inline int getZ2ID() const { return z2; }
  inline int getNumberOfVertices() const { return nVerts; }
  inline void setZ1ID( const int &z ) { z1 = z; }
  inline void setZ2ID( const int &z ) { z2 = z; }
  inline void setVertex( int v, int vid ) { vertices[v]=vid; }

  inline void reverseVertices() 
  {
    int vold[4];
    int v;
    for ( v=0; v<nVerts; v++ ) vold[v] = vertices[v];

    for ( v=0; v<nVerts; v++ ) vertices[v] = vold[nVerts-v-1];
  }

protected: 
  Face() { }
  int nVerts;
  int id;
  int vertices[4];
  int z1;
  int z2;
};

inline bool faceVerticesAreSame( const Face & face1, const Face & face2 )
{
  // this function checks to see if two faces are the same by examining the vertex lists
  // two faces are considered identicle if they both have the same vertices.  
  // the ordering of the vertices can be the same as or the reverse of each other.

  // first make sure the two faces have the same number of vertices
  bool result = face1.getNumberOfVertices() == face2.getNumberOfVertices();

  // if the number of vertices are the same, check the id's of each vertex
  if (result)
    {
      int nVertices = face1.getNumberOfVertices();
      int minVID1 = face1.getVertex(0);
      int minVIDX1 = 0;
      int minVID2 = face2.getVertex(0);
      int minVIDX2 = 0;
      // first find the starting point for each vertex
      // the starting point is the lowest value of a vertex id in a face
      for ( int v=1; v<nVertices; v++ )
	{
	  if (face1.getVertex(v)<minVID1)
	    {
	      minVID1 = face1.getVertex(v);
	      minVIDX1 = v;
	    }

	  if (face2.getVertex(v)<minVID2)
	    {
	      minVID2 = face2.getVertex(v);
	      minVIDX2 = v;
	    }
	}

      result = minVID1 == minVID2; 
      // if the starting vertices are not the same then the faces cannot be identicle
      if (result)
	{ 
	  // the starting vertices are the same and we now need to compare them
	  // check in the order given in each of the faces
	  for (int v=0; v<nVertices && result ; v++)
	    {
	      int if1 = (minVIDX1 + v)%nVertices;
	      int if2 = (minVIDX2 + v)%nVertices;
	      result = face1.getVertex(if1)==face2.getVertex(if2);
	    }
	  // if the check failed with the current ordering of each face, check with face2's ordering reversed
	  if (!result)
	    {
	      result = true;
	      for (int v=0; v<nVertices ; v++ )//&& result ; v++)
		{
		  int if1 = (minVIDX1 + v)%nVertices;
		  int if2 = (nVertices+minVIDX2 - v)%nVertices;
		  result = result && face1.getVertex(if1)==face2.getVertex(if2);
		  //result = face1.getVertex(if1)==face2.getVertex(if2);
		}
	    }
	}
    }

  return result;
}


#endif
