# Networking Laboratory

Welcome to the Networking Laboratory repository! This repository contains code and resources related to networking concepts and protocols.

## Table of Contents

- [Introduction](#introduction)
- [Installation](#installation)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)

## Introduction

This repository serves as a learning resource for networking enthusiasts. It provides examples, tutorials, and code snippets to help you understand various networking concepts like Socket Programming.

## Installation

To get started with the Networking Laboratory repository, follow these steps:

1. Clone the repository: 
```
git clone https://github.com/dhwanish-3/Networking_Laboratory.git
```

2. Change into the project directory: 
```
cd Networking_Laboratory
```
3. Compile and run server and client programs:
    - Manually
    ```
    cd <foldername>
    gcc -o <server-object-name> <server-name>
    gcc -o <client-object-name> <client-name>
    ./<server-object-name>
    ./<client-object-name> <server-ip-address>
    ```
    - Using Makefile
    ```
    cd <foldername>
    make
    ./<server-object-name>
    ./<client-object-name> <server-ip-address>
    ```
    - Save the file you want to run. Compile and run the latest modified file using bash script
    ```
    ./run.sh
    ```
    - Save a file in any folder which contains *Makefile*
    ```
    ./make_run.sh
    ```

## Usage

The Networking Laboratory repository offers a variety of resources to enhance your understanding of networking. Here are a few ways you can make use of this repository:

- Explore the code examples in the directory you wish to see practical implementations of networking concepts.

- Contribute your own code or documentation to help expand the repository and share your knowledge with others.

## Contributing

Contributions to the Networking Laboratory repository are welcome! If you would like to contribute, please follow these guidelines:

1. Fork the repository and create a new branch for your contribution.
2. Make your changes and ensure they adhere to the repository's coding style and guidelines.
3. Test your changes thoroughly.
4. Submit a pull request, describing the changes you have made and why they should be merged.

## License

The Networking Laboratory repository is licensed under the [MIT License](LICENSE). Please refer to the [LICENSE](LICENSE) file for more information.
