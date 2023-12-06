#include "common/common.h"
#include "common/json.h"
#include "openssl.h"
#include "macho.h"
#include "bundle.h"
#include <libgen.h>
#include <dirent.h>
#include <getopt.h>

const struct option options[] = {
	{"debug", no_argument, NULL, 'd'},
	{"force", no_argument, NULL, 'f'},
	{"verbose", no_argument, NULL, 'v'},
	{"cert", required_argument, NULL, 'c'},
	{"pkey", required_argument, NULL, 'k'},
	{"prov", required_argument, NULL, 'm'},
	{"password", required_argument, NULL, 'p'},
	{"bundle_id", required_argument, NULL, 'b'},
	{"bundle_name", required_argument, NULL, 'n'},
	{"bundle_version", required_argument, NULL, 'r'},
	{"entitlements", required_argument, NULL, 'e'},
	{"output", required_argument, NULL, 'o'},
	{"zip_level", required_argument, NULL, 'z'},
	{"dylib", required_argument, NULL, 'l'},
	{"weak", no_argument, NULL, 'w'},
	{"install", no_argument, NULL, 'i'},
	{"quiet", no_argument, NULL, 'q'},
	{"help", no_argument, NULL, 'h'},
	{}};

int usage()
{
	ZLog::Print("Usage: zsign [-options] [-k privkey.pem] [-m dev.prov] [-o output.ipa] file|folder\n");
	ZLog::Print("options:\n");
	ZLog::Print("-k, --pkey\t\tPath to private key or p12 file. (PEM or DER format)\n");
	ZLog::Print("-m, --prov\t\tPath to mobile provisioning profile.\n");
	ZLog::Print("-c, --cert\t\tPath to certificate file. (PEM or DER format)\n");
	ZLog::Print("-d, --debug\t\tGenerate debug output files. (.zsign_debug folder)\n");
	ZLog::Print("-f, --force\t\tForce sign without cache when signing folder.\n");
	ZLog::Print("-o, --output\t\tPath to output ipa file.\n");
	ZLog::Print("-p, --password\t\tPassword for private key or p12 file.\n");
	ZLog::Print("-b, --bundle_id\t\tNew bundle id to change.\n");
	ZLog::Print("-n, --bundle_name\tNew bundle name to change.\n");
	ZLog::Print("-r, --bundle_version\tNew bundle version to change.\n");
	ZLog::Print("-e, --entitlements\tNew entitlements to change.\n");
	ZLog::Print("-z, --zip_level\t\tCompressed level when output the ipa file. (0-9)\n");
	ZLog::Print("-l, --dylib\t\tPath to inject dylib file.\n");
	ZLog::Print("-w, --weak\t\tInject dylib as LC_LOAD_WEAK_DYLIB.\n");
	ZLog::Print("-i, --install\t\tInstall ipa file using ideviceinstaller command for test.\n");
	ZLog::Print("-q, --quiet\t\tQuiet operation.\n");
	ZLog::Print("-v, --version\t\tShows version.\n");
	ZLog::Print("-h, --help\t\tShows help (this message).\n");

	return -1;
}

int zsign(int argc, char *argv[])
{
	ZTimer gtimer;

	bool bForce = false;
	bool bInstall = false;
	bool bWeakInject = false;
	uint32_t uZipLevel = 0;

	string strCertFile;
	string strPKeyFile;
	string strProvFile;
	string strPassword;
	string strBundleId;
	string strBundleVersion;
	string strDyLibFile;
	string strOutputFile;
	string strDisplayName;
	string strEntitlementsFile;

	int opt = 0;
	int argslot = -1;
	while (-1 != (opt = getopt_long(argc, argv, "dfvhc:k:m:o:ip:e:b:n:z:ql:w", options, &argslot)))
	{
		switch (opt)
		{
		case 'c':
			strCertFile = optarg;
			break;
		case 'k':
			strPKeyFile = optarg;
			break;
		case 'm':
			strProvFile = optarg;
			break;
		case 'p':
			strPassword = optarg;
			break;
		}

		ZLog::DebugV(">>> Option:\t-%c, %s\n", opt, optarg);
	}

	if (optind >= argc)
	{
		return usage();
	}

	if (ZLog::IsDebug())
	{
		CreateFolder("./.zsign_debug");
		for (int i = optind; i < argc; i++)
		{
			ZLog::DebugV(">>> Argument:\t%s\n", argv[i]);
		}
	}

	string strPath = GetCanonicalizePath(argv[optind]);
	if (!IsFileExists(strPath.c_str()))
	{
		ZLog::ErrorV(">>> Invalid Path! %s\n", strPath.c_str());
		return -1;
	}
	ZTimer timer;
	ZSignAsset zSignAsset;
	if (!zSignAsset.Init(strCertFile, strPKeyFile, strProvFile, strEntitlementsFile, strPassword)) {
	    return -1;
	}
	bool bEnableCache = true;
	string strFolder = strPath;
	timer.Reset();
	ZAppBundle bundle;
	bool bRet = bundle.SignFolder(&zSignAsset, strFolder, strBundleId, strBundleVersion, strDisplayName, strDyLibFile, bForce, bWeakInject, bEnableCache);
	timer.PrintResult(bRet, ">>> Signed %s!", bRet ? "OK" : "Failed");
	gtimer.Print(">>> Done.");
	return bRet ? 0 : -1;
}
