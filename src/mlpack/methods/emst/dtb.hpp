/**
 * @file dtb.hpp
 * @author Bill March (march@gatech.edu)
 *
 * Contains an implementation of the DualTreeBoruvka algorithm for finding a
 * Euclidean Minimum Spanning Tree using the kd-tree data structure.
 *
 * Citation: March, W. B.; Ram, P.; and Gray, A. G.  Fast Euclidean Minimum
 * Spanning Tree: Algorithm, Analysis, Applications.  In KDD, 2010.
 *
 */
#ifndef __MLPACK_METHODS_EMST_DTB_HPP
#define __MLPACK_METHODS_EMST_DTB_HPP

#include "edge_pair.hpp"

#include <mlpack/core.hpp>
#include <mlpack/core/tree/bounds.hpp>
#include <mlpack/core/tree/binary_space_tree.hpp>
#include <mlpack/core/metrics/lmetric.hpp>

namespace mlpack {
namespace emst {

/**
 * A statistic for use with MLPACK trees, which stores the upper bound on
 * distance to nearest neighbors and the component which this node belongs to.
 */
class DTBStat
{
 private:
  //! Upper bound on the distance to the nearest neighbor of any point in this
  //! node.
  double maxNeighborDistance;
  //! The index of the component that all points in this node belong to.  This
  //! is the same index returned by UnionFind for all points in this node.  If
  //! points in this node are in different components, this value will be
  //! negative.
  int componentMembership;

 public:
  /**
   * A generic initializer.
   */
  DTBStat();

  /**
   * An initializer for leaves.
   */
  template<typename MatType>
  DTBStat(const MatType& dataset, const size_t start, const size_t count);

  /**
   * An initializer for non-leaves.
   */
  template<typename MatType>
  DTBStat(const MatType& dataset, const size_t start, const size_t count,
          const DTBStat& leftStat, const DTBStat& rightStat);

  //! Get the maximum neighbor distance.
  double MaxNeighborDistance() const { return maxNeighborDistance; }
  //! Modify the maximum neighbor distance.
  double& MaxNeighborDistance() { return maxNeighborDistance; }

  //! Get the component membership of this node.
  int ComponentMembership() const { return componentMembership; }
  //! Modify the component membership of this node.
  int& ComponentMembership() { return componentMembership; }

}; // class DTBStat

/**
 * Performs the MST calculation using the Dual-Tree Boruvka algorithm, using any
 * type of tree.  At the moment this class does not support arbitrary distance
 * metrics, and uses the squared Euclidean distance.
 *
 * For more information on the algorithm, see the following citation:
 *
 * @inproceedings{
 *   author = {March, W.B., Ram, P., and Gray, A.G.},
 *   title = {{Fast Euclidean Minimum Spanning Tree: Algorithm, Analysis,
 *      Applications.}},
 *   booktitle = {Proceedings of the 16th ACM SIGKDD International Conference
 *      on Knowledge Discovery and Data Mining}
 *   series = {KDD '10},
 *   year = {2010}
 * }
 *
 * @tparam TreeType Type of tree to use.
 */
template<
  typename TreeType = tree::BinarySpaceTree<bound::HRectBound<2>, DTBStat>
>
class DualTreeBoruvka
{
 private:
  //! Copy of the data (if necessary).
  typename TreeType::Mat dataCopy;
  //! Reference to the data (this is what should be used for accessing data).
  typename TreeType::Mat& data;

  //! Pointer to the root of the tree.
  TreeType* tree;
  //! Indicates whether or not we "own" the tree.
  bool ownTree;

  //! Indicates whether or not O(n^2) naive mode will be used.
  bool naive;

  //! Edges.
  std::vector<EdgePair> edges; // must use vector with non-numerical types

  //! Connections.
  UnionFind connections;

  //! Permutations of points during tree building.
  std::vector<size_t> oldFromNew;
  //! List of edge nodes.
  arma::Col<size_t> neighborsInComponent;
  //! List of edge nodes.
  arma::Col<size_t> neighborsOutComponent;
  //! List of edge distances.
  arma::vec neighborsDistances;

  //! Total distance of the tree.
  double totalDist;

  // For sorting the edge list after the computation.
  struct SortEdgesHelper
  {
    bool operator()(const EdgePair& pairA, const EdgePair& pairB)
    {
      return (pairA.Distance() < pairB.Distance());
    }
  } SortFun;

 public:
  /**
   * Create the tree from the given dataset.  This copies the dataset to an
   * internal copy, because tree-building modifies the dataset.
   *
   * @param data Dataset to build a tree for.
   * @param naive Whether the computation should be done in O(n^2) naive mode.
   * @param leafSize The leaf size to be used during tree construction.
   */
  DualTreeBoruvka(const typename TreeType::Mat& dataset,
                  const bool naive = false,
                  const size_t leafSize = 1);

  /**
   * Create the DualTreeBoruvka object with an already initialized tree.  This
   * will not copy the dataset, and can save a little processing power.  Naive
   * mode is not available as an option for this constructor; instead, to run
   * naive computation, construct a tree with all the points in one leaf (i.e.
   * leafSize = number of points).
   *
   * @note
   * Because tree-building (at least with BinarySpaceTree) modifies the ordering
   * of a matrix, be sure you pass the modified matrix to this object!  In
   * addition, mapping the points of the matrix back to their original indices
   * is not done when this constructor is used.
   * @endnote
   *
   * @param tree Pre-built tree.
   * @param dataset Dataset corresponding to the pre-built tree.
   */
  DualTreeBoruvka(TreeType* tree, const typename TreeType::Mat& dataset);

  /**
   * Delete the tree, if it was created inside the object.
   */
  ~DualTreeBoruvka();

  /**
   * Iteratively find the nearest neighbor of each component until the MST is
   * complete.  The results will be a 3xN matrix (with N equal to the number of
   * edges in the minimum spanning tree).  The first row will contain the lesser
   * index of the edge; the second row will contain the greater index of the
   * edge; and the third row will contain the distance between the two edges.
   *
   * @param results Matrix which results will be stored in.
   */
  void ComputeMST(arma::mat& results);

 private:
  /**
   * Adds a single edge to the edge list
   */
  void AddEdge(const size_t e1, const size_t e2, const double distance);

  /**
   * Adds all the edges found in one iteration to the list of neighbors.
   */
  void AddAllEdges();

  /**
   * Handles the base case computation.  Also called by naive.
   */
  double BaseCase(const TreeType* queryNode, const TreeType* referenceNode);

  /**
   * Handles the recursive calls to find the nearest neighbors in an iteration
   */
  void DualTreeRecursion(TreeType *queryNode,
                         TreeType *referenceNode,
                         double incomingDistance);

  /**
   * Unpermute the edge list and output it to results.
   */
  void EmitResults(arma::mat& results);

  /**
   * This function resets the values in the nodes of the tree nearest neighbor
   * distance, and checks for fully connected nodes.
   */
  void CleanupHelper(TreeType* tree);

  /**
   * The values stored in the tree must be reset on each iteration.
   */
  void Cleanup();

}; // class DualTreeBoruvka

}; // namespace emst
}; // namespace mlpack

#include "dtb_impl.hpp"

#endif // __MLPACK_METHODS_EMST_DTB_HPP
