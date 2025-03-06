package main

import (
	"fmt"
	"os"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: go run . <command> [args...]")
		os.Exit(1)
	}

	command := os.Args[1]

	switch command {
	case "b":
		build()
	case "gpf":
		generateProjectFiles()
	case "rls":
		if len(os.Args) < 4 {
			fmt.Println("Usage: go run . rls /Game/<level> <numplayers> <gamemode-without-leading-A>")
			os.Exit(1)
		}

		if len(os.Args) == 4 {
			runListenServer(RunConfig{os.Args[2], toIntRanged(os.Args[3], 1, 4), ""})
		} else {
			runListenServer(RunConfig{os.Args[2], toIntRanged(os.Args[3], 1, 4), os.Args[4]})
		}
	case "rsa":
		if len(os.Args) < 4 {
			fmt.Println("Usage: go run . rsa /Game/<level> <numplayers>")
			os.Exit(1)
		}

		runStandalone(RunConfig{os.Args[2], toIntRanged(os.Args[3], 1, 4), ""})
	case "uts":
		uploadToStore()
	default:
		fmt.Println("Unknown command:", command)
		os.Exit(1)
	}
}
