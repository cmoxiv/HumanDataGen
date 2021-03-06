/* *************************************************
 *
 * Copyright (2011) Willow Garage
 *
 * Author : Cedric Cagniart 
 * ************************************************* */

#include <libBVH/BVHMesh.h>

#include <map>
#include <fstream>
#include <stdexcept>

namespace BVH
{


void loadMesh_Tri( const char*            filename,
                   std::vector<Triangle>& tris )
{
	std::ifstream fin(filename);
	if( !fin.is_open() ) throw( std::runtime_error(std::string("(E)could not open: ") + std::string(filename)));

	int numTris = 0;
	fin >> numTris;
	tris.resize(numTris);

	for(int ti=0;ti<numTris;++ti) {
		fin >> tris[ti].v0 >> tris[ti].v1 >> tris[ti].v2;
	}

	if (fin.fail() ) throw( std::runtime_error(std::string("(E) malformed .tri file: ") + std::string(filename) ));
}


void loadMesh_labels( const char*              filename,
                      std::vector<int>&        labels )
{
	std::ifstream fin(filename);
	if( !fin.is_open() ) throw( std::runtime_error(std::string("(E)could not open: ") + std::string(filename) ));

	int numVertices = 0;
	fin >> numVertices;
	labels.resize(numVertices);

	labels.resize(numVertices);
	for(int vi=0;vi<numVertices;++vi){
		fin >> labels[vi];
	}

	if (fin.fail() ) throw( std::runtime_error(std::string("(E) malformed .labels file: ") + std::string(filename) ));


}

void loadMesh_VJ( const char*                  filename,
                  const std::vector<bvhJoint>& joints,
                  std::vector<int>&            vjs )
{
	std::ifstream fin(filename);
	if( !fin.is_open() ) throw( std::runtime_error(std::string("(E)could not open: ") + std::string(filename) ));

	int numVertices = 0;
	fin >> numVertices;
	vjs.resize(numVertices);


	int numJoints = joints.size();
	std::map<std::string, int> name_2_id;
	for( int ji=0;ji<numJoints;++ji) {
		name_2_id[joints[ji].name] = ji;
	}

	vjs.resize(numVertices);
	for(int vi=0;vi<numVertices;++vi){
		std::string name;
		fin >> name;
		std::map<std::string, int>::const_iterator f_itr = name_2_id.find(name);
		if( f_itr == name_2_id.end() ) throw ( std::runtime_error(std::string("(E) apparently mismatched joints and .bones files") + std::string(filename) ));
		vjs[vi] = f_itr->second;
	}

	if (fin.fail() ) throw( std::runtime_error(std::string("(E) malformed .bones file: ") + std::string(filename) ));

}


void loadMesh_Vertices( const char*          filename,
                        std::vector<Vec3>&   X0 )
{
	std::ifstream fin(filename);
	if( !fin.is_open() ) throw( std::runtime_error(std::string("(E)could not open: ") + std::string(filename) ));

	int numVertices = 0;
	fin >> numVertices;
	X0.resize(numVertices);

	for(int vi=0;vi<numVertices;++vi){
		fin >> X0[vi](0) >> X0[vi](1) >> X0[vi](2);
	}

	if (fin.fail() ) throw( std::runtime_error(std::string("(E) malformed .verts file: ") + std::string(filename) ));
}




void transformMesh( const std::vector<Transform3>& transforms,
                    const std::vector<int>&        VJ,
                    const std::vector<Vec3>&       X0,
                    std::vector<Vec3>&             X )
{
	int numVertices = X0.size();

	X.resize(numVertices);
	for(int vi=0;vi<numVertices;++vi) {
		X[vi] = transforms[VJ[vi]] * X0[vi];
	}
}





} // end namespace BVH


