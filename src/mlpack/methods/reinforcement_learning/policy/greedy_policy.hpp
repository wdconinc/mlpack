/**
 * @file greedy_policy.hpp
 * @author Shangtong Zhang
 *
 * This file is an implementation of epsilon greedy policy.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef MLPACK_METHODS_RL_POLICY_GREEDY_POLICY_HPP
#define MLPACK_METHODS_RL_POLICY_GREEDY_POLICY_HPP

#include <mlpack/prereqs.hpp>

namespace mlpack {
namespace rl {

/**
 * Implementation for epsilon greedy policy.
 *
 * In general we will select an action greedily based on the action value,
 * however under sometimes we will also randomly select an action to
 * encourage exploration.
 *
 * @tparam EnvironmentType The reinforcement learning task.
 */
template <typename EnvironmentType>
class GreedyPolicy {
 public:
  using ActionType = typename EnvironmentType::Action;

  /**
   * Constructor for epsilon greedy policy class.
   * @param initialEpsilon The initial probability to explore (select a random action).
   * @param annealInterval The steps during which the probability to explore will anneal.
   * @param minEpsilon Epsilon will never be less than this value.
   */
  GreedyPolicy(double initialEpsilon,
               size_t annealInterval,
               double minEpsilon) :
      epsilon(initialEpsilon),
      minEpsilon(minEpsilon),
      delta((initialEpsilon - minEpsilon) / annealInterval)
  { /* Nothing to do here. */ }

  /**
   * Sample an action based on given action values.
   * @param actionValue Values for each action.
   * @return Sampled action
   */
  ActionType Sample(const arma::colvec& actionValue)
  {
    double exploration = math::Random();

    // Select the action randomly.
    if (exploration < epsilon)
      return static_cast<ActionType>(math::RandInt(ActionType::size));

    // Select the action greedily.
    size_t bestAction = 0;
    double maxActionValue = actionValue[0];
    for (size_t action = 1; action < ActionType::size; ++action)
    {
      if (maxActionValue < actionValue[action])
      {
        maxActionValue = actionValue[action];
        bestAction = action;
      }
    }
    return static_cast<ActionType>(bestAction);
  };

  /**
   * Exploration probability will anneal at each step.
   */
  void Anneal()
  {
    epsilon -= delta;
    epsilon = std::max(minEpsilon, epsilon);
  }

  /**
   * @return Current possibility to explore.
   */
  const double& Epsilon() const { return epsilon; }

 private:
  //! Locally-stored probability to explore.
  double epsilon;

  //! Locally-stored lower bound for epsilon.
  double minEpsilon;

  //! Locally-stored stride for epsilon to anneal.
  double delta;
};

} // namespace rl
} // namespace mlpack

#endif
