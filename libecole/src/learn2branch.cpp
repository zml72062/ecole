#include <scip/scip.h>

#include "ecole/learn2branch.hpp"

namespace ecole {

BranchEnv::BranchEnv(scip::Model&& other_model) noexcept :
	model(std::move(other_model)) {}

void BranchEnv::run(
	std::function<std::size_t(std::unique_ptr<Observation>)> const& func) {
	// FIXME avoid having a copy here (model) and in the cache store (for different files.
	auto disposable_model = model;
	auto const branch_rule = [func](scip::Model const& model) {
		auto const var_idx = func(std::make_unique<Observation>());
		return model.lp_branch_vars()[var_idx];
	};
	disposable_model.set_branch_rule(branch_rule);
	disposable_model.solve();
}

} // namespace ecole