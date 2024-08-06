#include "UnusedForLoopVar.h"

#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendPluginRegistry.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

using namespace llvm;
using namespace clang;

//===----------------------------------------------------------------------===//
// Command line options
//===----------------------------------------------------------------------===//
static llvm::cl::OptionCategory UFLVCategory("uflv options");

//-----------------------------------------------------------------------------
// FrontendAction
//-----------------------------------------------------------------------------
class UFLVMatcherPluginAction : public PluginASTAction {
public:
  // Our plugin can alter behavior based on the command line options
  bool ParseArgs(const CompilerInstance &,
                 const std::vector<std::string> &) override {
    return true;
  }

  // Returns our ASTConsumer per translation unit.
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {
    return std::make_unique<UnusedForLoopVarASTConsumer>(CI.getASTContext(),
                                                         CI.getSourceManager());
  }
};

//-----------------------------------------------------------------------------
// Registration
//-----------------------------------------------------------------------------
static FrontendPluginRegistry::Add<UFLVMatcherPluginAction>
    X(/*Name=*/"UFLV",
      /*Desc=*/"Find unused for-loop variables");

//===----------------------------------------------------------------------===//
// Main driver code.
//===----------------------------------------------------------------------===//
int main(int argc, const char **argv) {
  Expected<tooling::CommonOptionsParser> OptParser =
      clang::tooling::CommonOptionsParser::create(argc, argv, UFLVCategory);
  if (auto E = OptParser.takeError()) {
    errs() << "Problem constructing CommonOptionsParser "
           << toString(std::move(E)) << '\n';
    return EXIT_FAILURE;
  }
  clang::tooling::ClangTool Tool(OptParser->getCompilations(),
                                 OptParser->getSourcePathList());

  return Tool.run(
      clang::tooling::newFrontendActionFactory<UFLVMatcherPluginAction>()
          .get());
}
