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

  DGP_CONSOLE << "Read mesh '" << mesh.getName() << "' with " << mesh.numVertices() << " vertices, " << mesh.numEdges()
              << " edges and " << mesh.numFaces() << " faces from " << in_path;

  Real d = mesh.getAverageDistance();
  double sigma_c = d/20;
  double sigma_s = d/3;

  mesh.save("./orig.off");
  mesh.noiseMesh(d/3);
  mesh.save("./noisy.off");
  
  Viewer viewer1;
  viewer1.setObject(&mesh, sigma_c,sigma_s);
  viewer1.launch(argc, argv);

  return 0;
}
