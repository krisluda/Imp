package main

import (
	"fmt"
	"os"
	"os/exec"
	"strconv"
	"strings"

	"github.com/joho/godotenv"
)

type Config struct {
	UnrealDir  string
	ProjectDir string
}

type StoreConfig struct {
	OrganizationID    string
	ProductID         string
	BuildPatchToolDir string
	ArtifactID        string
	BptClientID       string
	BptClientSecret   string
	BuildDir          string
	BuildDirCloud     string
}

type RunConfig struct {
	Level      string
	NumPlayers int
	GameMode   string
}

type Version struct {
	Major int
	Minor int
	Patch int
}

func exitOnError(err error) {
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}

func toInt(s string) int {
	i, err := strconv.Atoi(s)
	exitOnError(err)
	return i
}

func toIntRanged(s string, min int, max int) int {
	i := toInt(s)
	if i < min || i > max {
		fmt.Println("Value must be between", min, "and", max)
		os.Exit(1)
	}
	return i
}

func getEnv(key string) string {
	value := os.Getenv(key)
	if value == "" {
		fmt.Println(key + " not set")
		os.Exit(1)
	}
	return value
}

func getConfig() Config {
	err := godotenv.Load()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	unrealDir := getEnv("UNREAL_DIR")
	projectDir := getEnv("PROJECT_DIR")

	return Config{unrealDir, projectDir}
}

func getStoreConfig() StoreConfig {
	err := godotenv.Load()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	organizationID := getEnv("ORGANIZATION_ID")
	productID := getEnv("PRODUCT_ID")
	buildPatchToolDir := getEnv("BUILD_PATCH_TOOL_DIR")
	artifactID := getEnv("ARTIFACT_ID")
	bptClientID := getEnv("BPT_CLIENT_ID")
	bptClientSecret := getEnv("BPT_CLIENT_SECRET")
	buildDir := getEnv("BUILD_DIR")
	buildDirCloud := getEnv("BUILD_DIR_CLOUD")

	return StoreConfig{organizationID, productID, buildPatchToolDir, artifactID, bptClientID, bptClientSecret, buildDir, buildDirCloud}
}

func runCommand(args ...string) {
	fmt.Println("Running command: ", strings.Join(args, " "))
	fmt.Println("")

	command := "cmd"
	firstArg := "/c"
	allArgs := append([]string{firstArg}, args...)

	cmd := exec.Command(command, allArgs...)
	cmd.Stdout = os.Stdout
	cmd.Stderr = os.Stderr
	err := cmd.Run()
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}
}
