//
// Created on 2/20/2022.
//

#ifndef RECOMMENDATIONSYSTEM_H
#define RECOMMENDATIONSYSTEM_H

#include <map>
#include <vector>
#include <cmath>
#include <algorithm>
#include <list>
#include <set>
#include "User.h"
#define MAGIC_ZERO 0.0;
class User;

struct comparator{
    bool operator()(const sp_movie& a, const sp_movie& b){
        return *a < *b;
    }
};

typedef std::pair<sp_movie , double> t_pair;
typedef std::map<sp_movie, std::vector<double>,comparator> movie_feature_map;
typedef std::pair<sp_movie, double> ms_pair;


class RecommendationSystem
{


private:
    movie_feature_map _movie_feature_map;

    std::vector<double> make_preference_vector(double mean,
                                                      const User& user);
    double calculate_mean(const User& user);
    double calc_similarity(const std::vector<double>&
            preferences, std::vector<double> unseen_movie_feat);
    double calculate_vector_norm(const std::vector<double>& vector);
    static bool compare_pairs_by_values_reverse
    (const t_pair& lhs, const t_pair& rhs);
    std::vector<ms_pair> get_k_best_values (std::vector<t_pair> pairs, int k);

public:

	//explicit RecommendationSystem()
    /**
     * adds a new movie to the system
     * @param name name of movie
     * @param year year it was made
     * @param features features for movie
     * @return shared pointer for movie in system
     */
	sp_movie add_movie(const std::string& name,
                       int year,const std::vector<double>& features);


    /**
     * a function that calculates the movie with highest
     * score based on movie features
     * @param ranks user ranking to use for algorithm
     * @return shared pointer to movie in system
     */
    sp_movie recommend_by_content(const User& user);

    /**
     * a function that calculates the movie with highest
     * predicted score based on ranking of other movies
     * @param ranks user ranking to use for algorithm
     * @param k
     * @return shared pointer to movie in system
     */
	sp_movie recommend_by_cf(const User& user, int k);


    /**
     * Predict a user rating for a movie given argument using
     * item cf procedure with k most similar movies.
     * @param user_rankings: ranking to use
     * @param movie: movie to predict
     * @param k:
     * @return score based on algorithm as described in pdf
     */
	double predict_movie_score(const User &user, const sp_movie &movie,
												  int k);

	/**
	 * gets a shared pointer to movie in system
	 * @param name name of movie
	 * @param year year movie was made
	 * @return shared pointer to movie in system
	 */
	sp_movie get_movie(const std::string &name, int year) ;


	friend std::ostream &operator<< (std::ostream& o,
            RecommendationSystem& self);

};


#endif //RECOMMENDATIONSYSTEM_H
