package main

import (
	"errors"
	"fmt"
	"io/fs"
	"os"
	"regexp"
	"strconv"
	"strings"
	"sync"
)

func build() {
	config := getConfig()

	runCommand(config.UnrealDir+"\\Engine\\Build\\BatchFiles\\Build.bat", "ImpEditor", "Win64", "Development", config.ProjectDir+"\\Imp.uproject", "-waitmutex", "-time")
}

func generateProjectFiles() {
	config := getConfig()

	runCommand(config.UnrealDir+"\\Engine\\Binaries\\DotNET\\UnrealBuildTool\\UnrealBuildTool.exe", "-projectfiles", "-project="+config.ProjectDir+"\\Imp.uproject", "-game", "-rocket", "-progress")
}

func runListenServer(runConfig RunConfig) {
	config := getConfig()

	build()
	var wg sync.WaitGroup

	for i := 0; i < runConfig.NumPlayers; i++ {
		playerNum := i + 1
		xPos := 10
		yPos := 40 + (i * 490)

		level := ""

		if i == 0 {
			level = runConfig.Level + "?listen"
			level += "?numplayers=" + strconv.Itoa(runConfig.NumPlayers)
		} else {
			level = "127.0.0.1:7777"
		}

		level += "?name=Player" + strconv.Itoa(playerNum)

		if runConfig.GameMode != "" {
			level += "?game=/Script/Imp." + runConfig.GameMode
		}

		wg.Add(1)

		go func() {
			defer wg.Done()

			runCommand(config.UnrealDir+"\\Engine\\Binaries\\Win64\\UnrealEditor.exe", config.ProjectDir+"\\Imp.uproject", level, "-game", "-log=Player"+strconv.Itoa(playerNum)+".log", "-windowed", "-winx="+fmt.Sprint(xPos), "-winy="+fmt.Sprint(yPos), "-resx=800", "-resy=450", "-nosplash", "-ini:Engine:[OnlineSubsystem]:DefaultPlatformService=null", "-ExecCmds=t.MaxFPS 30, Stat FPS", "-ExecCmds=open "+runConfig.Level)
		}()
	}

	wg.Wait()
}

func runStandalone(runConfig RunConfig) {
	config := getConfig()

	build()
	var wg sync.WaitGroup

	for i := 0; i < runConfig.NumPlayers; i++ {
		playerNum := i + 1
		xPos := 10
		yPos := 40 + (i * 490)

		wg.Add(1)

		go func() {
			defer wg.Done()

			runCommand(config.UnrealDir+"\\Engine\\Binaries\\Win64\\UnrealEditor.exe", config.ProjectDir+"\\Imp.uproject", runConfig.Level+"?name=Player"+strconv.Itoa(playerNum), "-game", "-log=Player"+strconv.Itoa(playerNum)+".log", "-windowed", "-winx="+fmt.Sprint(xPos), "-winy="+fmt.Sprint(yPos), "-resx=800", "-resy=450", "-nosplash", "-ini:Engine:[OnlineSubsystem]:DefaultPlatformService=null", "-ExecCmds=t.MaxFPS 30, Stat FPS", "-ExecCmds=open "+runConfig.Level)
		}()
	}

	wg.Wait()
}

func versionToString(version Version) string {
	return fmt.Sprintf("%d.%d.%d", version.Major, version.Minor, version.Patch)
}

func uploadToStore() {
	config := getStoreConfig()

	versionFile, err := os.ReadFile("../../build_version")
	if err != nil {
		if errors.Is(err, fs.ErrNotExist) {
			fmt.Println("build_version file does not exist")
			os.Exit(1)
		}
		fmt.Println("error reading build_version: %w", err)
		os.Exit(1)
	}

	versionStr := string(versionFile)
	if versionStr == "" {
		fmt.Println("build_version is empty")
		os.Exit(1)
	}

	if !regexp.MustCompile(`^\d+\.\d+\.\d+$`).MatchString(versionStr) {
		fmt.Println("build_version is not in correct format")
		os.Exit(1)
	}

	versionParts := strings.Split(string(versionFile), ".")
	major, err := strconv.Atoi(versionParts[0])
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	minor, err := strconv.Atoi(versionParts[1])
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	patch, err := strconv.Atoi(versionParts[2])
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
	version := Version{major, minor, patch}

	fmt.Printf("Current version: %s\n", versionToString(version))
	fmt.Print("Bump patch, minor, or major (patch/minor/major)?\n")
	input := ""
	fmt.Scanln(&input)

	if input == "patch" {
		version.Patch++
	} else if input == "minor" {
		version.Minor++
		version.Patch = 0
	} else if input == "major" {
		version.Major++
		version.Minor = 0
		version.Patch = 0
	} else {
		fmt.Println("No version bump")
		return
	}

	fmt.Printf("New version: %s\n", versionToString(version))
	fmt.Print("Continue (y/n)?\n")
	input = ""
	fmt.Scanln(&input)

	if input != "y" {
		fmt.Println("Aborting")
		return
	}

	runCommand(config.BuildPatchToolDir+"\\Engine\\Binaries\\Win64\\BuildPatchTool.exe", "-OrganizationId="+config.OrganizationID, "-ProductId="+config.ProductID, "-ArtifactId="+config.ArtifactID, "-ClientId="+config.BptClientID, "-ClientSecret="+config.BptClientSecret, "-BuildRoot="+config.BuildDir, "-CloudDir="+config.BuildDirCloud, "-BuildVersion="+versionToString(version), "-AppLaunch=Imp.exe", "-AppArgs=", "-FileAttributeList=", "-FileIgnoreList=")

	os.WriteFile("../../build_version", []byte(versionToString(version)), 0644)
	fmt.Printf("Wrote version: %s\n", versionToString(version))
}
