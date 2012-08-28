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

#if 1
#include "clang/Lex/HeaderSearch.h"
#include "clang/Frontend/HeaderSearchOptions.h"
#include "clang/Frontend/Utils.h"
#endif

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

#if 1
	ci.getPreprocessorOpts().UsePredefines = true;

	clang::HeaderSearch headerSearch(ci.getFileManager(),
							  ci.getDiagnostics(),
							  ci.getLangOpts(),
							  pti);

	clang::HeaderSearchOptions headerSearchOptions;

	// <Warning!!> -- Platform Specific Code lives here
	// This depends on A) that you're running linux and
	// B) that you have the same GCC LIBs installed that
	// I do.
	// Search through Clang itself for something like this,
	// go on, you won't find it. The reason why is Clang
	// has its own versions of std* which are installed under
	// /usr/local/lib/clang/<version>/include/
	// See somewhere around Driver.cpp:77 to see Clang adding
	// its version of the headers to its include path.
	headerSearchOptions.AddPath("/usr/include",
								clang::frontend::Angled,
								false,
								false,
								false);
	headerSearchOptions.AddPath("/usr/lib/gcc/x86_64-linux-gnu/4.4.5/include",
								clang::frontend::Angled,
								false,
								false,
								false);
	// </Warning!!> -- End of Platform Specific Code

	clang::InitializePreprocessor(ci.getPreprocessor(),
								  ci.getPreprocessorOpts(),
								  headerSearchOptions,
	ci.getFrontendOpts());
#endif

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
