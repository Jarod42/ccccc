
#if 0
#include <clang-c/Index.h>

#define ARRAY_SIZE(a) (sizeof (a) / sizeof(*a))
void foo()
{
	CXIndex index = clang_createIndex(1, 1);

	// Support Microsoft extensions
	const char *args[] = {"-fms-extensions"};

	CXTranslationUnit tu = clang_parseTranslationUnit(index, "mySource.c", args, ARRAY_SIZE(args), 0, 0, 0);

	if (tu)
	{
		CXCursor cursor = clang_getTranslationUnitCursor(tu);

		(void) cursor;
		// Use the cursor functions to navigate through the AST
	}
}

#endif

#if 1


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"

#include "llvm/Support/Host.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Basic/TargetOptions.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/FileManager.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Parse/Parser.h"

#pragma GCC diagnostic pop

#include <iostream>

void bar()
{
	clang::CompilerInstance ci;
	ci.createDiagnostics(0, NULL);

	clang::TargetOptions to;
	to.Triple = llvm::sys::getDefaultTargetTriple();
	clang::TargetInfo *pti = clang::TargetInfo::CreateTargetInfo(ci.getDiagnostics(), to);
	ci.setTarget(pti);

	ci.createFileManager();
	ci.createSourceManager(ci.getFileManager());
	ci.createPreprocessor();

	clang::ASTConsumer *astConsumer = new clang::ASTConsumer();
	ci.setASTConsumer(astConsumer);

	ci.createASTContext();
	ci.createSema(clang::TU_Complete, NULL);

	const clang::FileEntry *pFile = ci.getFileManager().getFile("test.c");
	assert(pFile);
	ci.getSourceManager().createMainFileID(pFile);
	ci.getPreprocessor().EnterMainSourceFile();
	ci.getDiagnosticClient().BeginSourceFile(ci.getLangOpts(), &ci.getPreprocessor());
#if 0
	clang::Token tok;
	do {
		ci.getPreprocessor().Lex(tok);
		if (ci.getDiagnostics().hasErrorOccurred()) {
			break;
		}
		ci.getPreprocessor().DumpToken(tok);
		std::cerr << std::endl;
	} while ( tok.isNot(clang::tok::eof));
	ci.getDiagnosticClient().EndSourceFile();
#else
	clang::Parser parser(ci.getPreprocessor(), ci.getSema(), false /*skipFunctionBodies*/);
	parser.ParseTranslationUnit();
	ci.getDiagnosticClient().EndSourceFile();
	ci.getASTContext().Idents.PrintStats();
#endif

}

#endif

int main()
{
	//foo();
	bar();
	return 0;
}







