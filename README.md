# TREE-tool
implement the "tree" tool in a long mode, printing the file tree with permissions, user and group, and size information.

## written by ✨

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://github.com/RazElbaz"><img src="https://avatars.githubusercontent.com/u/93310416?v=4" width="100px;" alt="Raz Elbaz"/><br /><sub><b>Raz Elbaz</b></sub></a><br /><a href="https://github.com/RazElbaz" title="Code">💻</a> <br /> </td>
  </tr>
</table>

# Task 3: stree - File Tree Printing Tool

## Description
The task requires implementing a file tree printing tool called "stree" in long mode. This tool should display the file tree structure along with permissions, user and group information, and size. The expected output format is similar to the provided picture.

The default behavior of the tool is to start from the current folder and print the file tree downward. To facilitate directory tree walking, two example files (ftw1.c and ftw2.c) are provided, and it is recommended to understand them before starting the implementation.

To create the special tree characters (-, |, etc.), you can directly copy them from the original tree terminal window.

## Instructions
1. Implement the "stree" tool to print the file tree structure in long mode.
2. Ensure that the output includes the following information for each file/directory:
   - Permissions: The permissions of the file/directory.
   - User: The user associated with the file/directory.
   - Group: The group associated with the file/directory.
   - Size: The size of the file/directory.
3. Start printing the tree structure from the current folder and move downward.
4. Copy the special tree characters (-, |, etc.) from the original tree terminal window for accurate representation.
5. Submit the following files:
   - The implementation code for the "stree" tool.
   - A working makefile to build the tool.

## Example Output
```
.
├── [-rwxr-xr-x  user1  group1  4096]  file1
├── [drwxr-xr-x  user1  group1  4096]  directory1
│   ├── [-rw-r--r--  user1  group1  1024]  file2
│   └── [-rw-r--r--  user1  group1  2048]  file3
└── [-rwxr-xr-x  user2  group2  8192]  file4
```

