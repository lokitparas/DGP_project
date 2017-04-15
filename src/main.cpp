#include "Mesh.hpp"
#include <algorithm>
#include <cstdlib>
#include <vector>
#include <random>
#include "DGP/VectorN.hpp"
#include "Viewer.hpp"

using namespace std;

int
usage(int argc, char * argv[])
{
  DGP_CONSOLE << "";
  DGP_CONSOLE << "Usage: " << argv[0] << " <mesh> [vol2bbox] [d2 <#points> <#bins>]";
  DGP_CONSOLE << "";

  return -1;
}

int
main(int argc, char * argv[])
{
  if (argc < 2)
    return usage(argc, argv);

  std::string in_path = argv[1];

  Mesh mesh;
  if (!mesh.load(in_path))
    return -1;

  std::cout << mesh.getAverageDistance() << std::endl;

  mesh.noiseMesh(0.005);
  mesh.save("./noisy.off");
  mesh.bilateralSmooth(0.005,0.05);
  mesh.save("./smooth.off");

  Mesh originalMesh, noisyMesh, smoothMesh;
  originalMesh.load(in_path);
  noisyMesh.load("./noisy.off");
  smoothMesh.load("./smooth.off");


  DGP_CONSOLE << "Read mesh '" << mesh.getName() << "' with " << mesh.numVertices() << " vertices, " << mesh.numEdges()
              << " edges and " << mesh.numFaces() << " faces from " << in_path;

  Viewer viewer1;
  viewer1.setObject(&originalMesh,&noisyMesh,&smoothMesh);
  viewer1.launch(argc, argv);

  // Viewer viewer2;
  // viewer2.setObject(&noised);
  // viewer2.launch(argc, argv);


  return 0;
}
