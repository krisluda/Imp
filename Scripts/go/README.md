# Unreal Tools

## Creation of Unreal Tools:

Install golang.

Install Go VSCode extension for syntax highlighting.

Create a new module:

`go mod init unreal-tools`

Add packages to module:

`go get <git repo address>`

Compile and run one file:

`go run <foo.go> [args...]`

Compile and run all files:

`go run . [args...]`

Build project. Extension .exe needed for Windows.

`go build -o unreal-tools.exe`
