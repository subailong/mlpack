/**
 * @file allkfn_test.cc
 *
 * Test file for AllkFN class
 */
#include <fastlib/fastlib.h>
#include <armadillo>
#include "allkfn.h"

#define BOOST_TEST_MODULE AllkFN Test
#include <boost/test/unit_test.hpp>

using namespace mlpack::allkfn;

/***
 * Test the dual-tree furthest-neighbors method with the naive method.  This
 * uses both a query and reference dataset.
 *
 * Errors are produced if the results are not identical.
 */
BOOST_AUTO_TEST_CASE(dual_tree_vs_naive_1) {
  arma::mat data_for_tree_;

  // Hard-coded filename: bad!
  if (data::Load("test_data_3_1000.csv", data_for_tree_) != SUCCESS_PASS)
    BOOST_FAIL("Cannot load test dataset test_data_3_1000.csv!");

  // Set up matrices to work with.
  arma::mat dual_query(data_for_tree_);
  arma::mat dual_references(data_for_tree_);
  arma::mat naive_query(data_for_tree_);
  arma::mat naive_references(data_for_tree_);

  AllkFN allkfn_(dual_query, dual_references, 20, 5);
  AllkFN naive_(naive_query, naive_references, 1 /* leaf_size ignored */, 5,
      AllkFN::NAIVE);
 
  arma::Col<index_t> resulting_neighbors_tree;
  arma::vec distances_tree;
  allkfn_.ComputeNeighbors(resulting_neighbors_tree, distances_tree);

  arma::Col<index_t> resulting_neighbors_naive;
  arma::vec distances_naive;
  naive_.ComputeNeighbors(resulting_neighbors_naive, distances_naive);

  for (index_t i = 0; i < resulting_neighbors_tree.n_elem; i++) {
    BOOST_REQUIRE(resulting_neighbors_tree[i] == resulting_neighbors_naive[i]);
    BOOST_REQUIRE_CLOSE(distances_tree[i], distances_naive[i], 1e-5);
  }
} 

/***
 * Test the dual-tree furthest-neighbors method with the naive method.  This
 * uses only a reference dataset.
 *
 * Errors are produced if the results are not identical.
 */
BOOST_AUTO_TEST_CASE(dual_tree_vs_naive_2) {
  arma::mat data_for_tree_;

  // Hard-coded filename: bad!
  // Code duplication: also bad!
  if (data::Load("test_data_3_1000.csv", data_for_tree_) != SUCCESS_PASS)
    BOOST_FAIL("Cannot load test dataset test_data_3_1000.csv!");

  // Set up matrices to work with (may not be necessary with no ALIAS_MATRIX?).
  arma::mat dual_references(data_for_tree_);
  arma::mat naive_references(data_for_tree_);

  AllkFN allkfn_(dual_references, 20, 5);
  AllkFN naive_(naive_references, 1 /* leaf_size ignored */, 5, AllkFN::NAIVE);

  arma::Col<index_t> resulting_neighbors_tree;
  arma::vec distances_tree;
  allkfn_.ComputeNeighbors(resulting_neighbors_tree, distances_tree);

  arma::Col<index_t> resulting_neighbors_naive;
  arma::vec distances_naive;
  naive_.ComputeNeighbors(resulting_neighbors_naive, distances_naive);

  for (index_t i = 0; i < resulting_neighbors_tree.n_elem; i++) {
    BOOST_REQUIRE(resulting_neighbors_tree[i] == resulting_neighbors_naive[i]);
    BOOST_REQUIRE_CLOSE(distances_tree[i], distances_naive[i], 1e-5);
  }
}

/***
 * Test the single-tree furthest-neighbors method with the naive method.  This
 * uses only a reference dataset.
 *
 * Errors are produced if the results are not identical.
 */
BOOST_AUTO_TEST_CASE(single_tree_vs_naive) {
  arma::mat data_for_tree_;
  
  // Hard-coded filename: bad!
  // Code duplication: also bad!
  if (data::Load("test_data_3_1000.csv", data_for_tree_) != SUCCESS_PASS)
    BOOST_FAIL("Cannot load test dataset test_data_3_1000.csv!");

  arma::mat single_query(data_for_tree_);
  arma::mat naive_query(data_for_tree_);

  AllkFN allkfn_(single_query, 20, 5, AllkFN::MODE_SINGLE);
  AllkFN naive_(naive_query, 1 /* leaf_size ignored */, 5, AllkFN::NAIVE);

  arma::Col<index_t> resulting_neighbors_tree;
  arma::vec distances_tree;
  allkfn_.ComputeNeighbors(resulting_neighbors_tree, distances_tree);

  arma::Col<index_t> resulting_neighbors_naive;
  arma::vec distances_naive;
  naive_.ComputeNeighbors(resulting_neighbors_naive, distances_naive);

  for (index_t i = 0; i < resulting_neighbors_tree.n_elem; i++) {
    BOOST_REQUIRE(resulting_neighbors_tree[i] == resulting_neighbors_naive[i]);
    BOOST_REQUIRE_CLOSE(distances_tree[i], distances_naive[i], 1e-5);
  }
}
