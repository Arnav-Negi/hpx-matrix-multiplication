//
// Created by arnav on 31/3/23.
//
#include <cstdint>
#include <hpx/init.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/local/init.hpp>
#include <hpx/local/execution.hpp>

int hpx_main(hpx::program_options::variables_map &vm) {
    std::uint64_t n = vm["n"].as<std::uint64_t>();
    std::uint64_t k = vm["k"].as<std::uint64_t>();
    std::uint64_t m = vm["m"].as<std::uint64_t>();
    std::uint64_t l = vm["l"].as<std::uint64_t>();
    std::uint64_t u = vm["u"].as<std::uint64_t>();

    assert(l <= u);

    // Timer to keep track of time elapsed.
    hpx::chrono::high_resolution_timer t;




    return hpx::local::finalize();
}

int main(int argc, char **argv) {
    using namespace hpx::program_options;
// Configure application-specific options
    options_description desc_commandline(
            "Usage: "
    HPX_APPLICATION_STRING
    " [options]");

    // Dimensions of the matrices.
    desc_commandline.add_options()
            ("n", value<std::uint64_t>()->default_value(3), "ows of first matrix")
            ("m", value<std::uint64_t>()->default_value(3), "Columns of first matrix, rows of second matrix")
            ("k", value<std::uint64_t>()->default_value(3), "Rows of second matrix")
            ("l", value<std::uint64_t>()->default_value(-10), "Lower bound on random entries of matrix")
            ("u", value<std::uint64_t>()->default_value(10), "Upper bound on random entries of matrix")

    // Initialize and run HPX
    hpx::local::init_params init_args;
    init_args.desc_cmdline = desc_commandline;

    return hpx::local::init(hpx_main, argc, argv, init_args);
}
