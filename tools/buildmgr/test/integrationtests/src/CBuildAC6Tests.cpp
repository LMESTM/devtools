/*
 * Copyright (c) 2020-2021 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/*
* Integration tests for cbuild.sh
*
* The tests validate different configurations and
* features of cbuild.sh with AC6 toolchain
*
*/

#include "CBuildTestFixture.h"

using namespace std;

class CBuildAC6Tests : public CBuildTestFixture {
public:
  void SetUp();
};

void CBuildAC6Tests::SetUp() {
  string toolchainPath = CrossPlatformUtils::GetEnv("CI_ARMCC6_TOOLCHAIN_ROOT");
  if (toolchainPath.empty() && !fs::exists(CBuildIntegTestEnv::ac6_toolchain_path)) {
    GTEST_SKIP();
  }
}

// Validate successful build of C example
// using latest pack versions available
TEST_F(CBuildAC6Tests, Build_AC6) {
  TestParam param = { "AC6/Build_AC6", "Simulation" };

  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);
}

// Validate successful generation and compilation
// of cprj using "--update" option with fixed pack versions
TEST_F(CBuildAC6Tests, Build_AC6_Fixed) {
  TestParam param = {
    "AC6/Build_AC6", "Simulation",
    "--update=Simulation.fixed.cprj", "", true
  };

  RunCBuildScriptClean       (param);
  RunCBuildScriptWithArgs    (param);

  param = { "AC6/Build_AC6", "Simulation.fixed" };
  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);
}

// Validate successful compilation of C++ example
TEST_F(CBuildAC6Tests, Build_AC6PP) {
  TestParam param = { "AC6/Build_AC6PP", "Simulation" };

  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);
}

// Validate project compilation with inclusion of
// files or directory paths having whitespaces
TEST_F(CBuildAC6Tests, Whitespace) {
  TestParam param = { "AC6/Whitespace", "Target_Name" };

  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);
}

// Validate project compilation having flag definitions and their usage
TEST_F(CBuildAC6Tests, Flags) {
  TestParam param = { "AC6/Flags", "Target" };

  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);
}

// Verify and build project with assembly files
TEST_F(CBuildAC6Tests, Asm) {
  TestParam param = { "AC6/Asm", "Target" };

  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);
}

// Validate project compilation having minimal configurations
TEST_F(CBuildAC6Tests, Minimal) {
  TestParam param = { "AC6/Minimal", "MyProject" };

  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);
}

// Validate project with usage of nested groups and
// inheritance of flags in each group
TEST_F(CBuildAC6Tests, NestedGroups) {
  TestParam param = { "AC6/NestedGroups", "Project" };

  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);
}

// Test project with directory inclusion with relative paths
TEST_F(CBuildAC6Tests, RelativePath) {
  TestParam param = { "AC6/RelativePath", "Project" };

  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);
}

// Validate compilation of library
TEST_F(CBuildAC6Tests, Library) {
  TestParam param = { "AC6/Library", "project" };

  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);
}

// Validate usage of secure objects in non secure project
TEST_F(CBuildAC6Tests, TrustZone_AC6) {
  TestParam param = { "AC6/TrustZone/CM33_s", "FVP_Simulation_Model" };

  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);

  param.name = "AC6/TrustZone/CM33_ns";
  RunCBuildScriptClean       (param);
  RunCBuildScript            (param);
  CheckCMakeLists            (param);
}

// Test building of multitoolchain project with AC6 compiler
// and defined output and intermediate directory paths
TEST_F(CBuildAC6Tests, MultiToolChainAC6Test) {
  const string outDir = "OutDir";
  const string intDir = "IntDir";
  const string toolchain = "AC6";
  TestParam param = { "Mixed/Build_AC6_GCC", "Build.Simulation", "--toolchain=" + toolchain + " --outdir=" + outDir + " --intdir=" + intDir, "", true };
  const string absOutDir = examples_folder + "/" + param.name + "/" + outDir;
  const string absIntDir = examples_folder + "/" + param.name + "/" + intDir;
  error_code ec;

  if (fs::exists(absOutDir, ec)) {
    RemoveDir(absOutDir);
  }

  if (fs::exists(absIntDir, ec)) {
    RemoveDir(absIntDir);
  }

  RunCBuildScriptWithArgs   (param);
  CheckOutputDir            (param, absOutDir);
  CheckCMakeIntermediateDir (param, absIntDir);
}

// Validate project with preincluded header paths and their usage
TEST_F(CBuildAC6Tests, PreInclude) {
  TestParam param = { "Mixed/Pre Include", "Target", "", "", true };

  RunScript("preinclude.sh", testout_folder);
  RunCBuildScriptClean      (param);
  RunCBuildScriptWithArgs   (param);
  CheckCMakeLists           (param);
}
