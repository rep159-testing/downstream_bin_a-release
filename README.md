# downstream_bin_a

The package of the `binext` distribution of the
[REP 159](https://github.com/openrobotics/reps/pull/32) test harness.
`binext` extends the `upstream` distribution with
`extension_method: binary_import`: `upstream_c`, `upstream_b` and
`upstream_a` are used as the binaries `upstream` releases, installed under
`/opt/ros/upstream`, and only this package is built. Same code as
`downstream_src_a`: reads a YAML document with libyaml. Plain CMake package,
no ROS dependency.

Index and distribution files: https://github.com/rep159-testing/rep159-rosdistro
