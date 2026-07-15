# weapi - Lightweight REST API Backend in C

[![Language](https://img.shields.io/badge/Language-C-blue.svg)](#)
[![Framework](https://img.shields.io/badge/Framework-facil.io-green.svg)](https://facil.io/)
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20WSL-orange.svg)](#)

A lightweight RESTful backend API developed in **C programming language** using the **facil.io framework**.

The project implements a simple user management backend with HTTP routing, JSON-based responses, CRUD operations, and thread-safe data handling.

This project was developed to explore backend development concepts, low-level network programming, HTTP servers, and system programming in C.

---

# 📌 Project Overview

Modern backend systems are commonly built using high-level frameworks, but implementing server-side applications in C provides deeper understanding of:

- HTTP communication
- Server architecture
- Memory management
- Data structures
- Thread synchronization
- Low-level programming concepts

`weapi` is a lightweight backend server that provides REST API endpoints for user management.

The server receives HTTP requests, processes user operations, and returns JSON responses.

---

# ✨ Features

## User Management System

The project supports:

- Creating new users
- User authentication
- Reading user information
- Updating user data
- Deleting users
- Retrieving users by ID


## Backend Features

- REST-style HTTP API
- JSON response format
- Lightweight C implementation
- Thread-safe shared data handling
- Custom user data structure
- Fast compilation using Makefile
- Linux/WSL compatibility

---

# 🛠 Technologies

| Technology | Description |
|---|---|
| C | Main programming language |
| facil.io | HTTP server framework |
| GCC | Compiler |
| Makefile | Build automation |
| Linux / WSL | Development environment |
| JSON | Data exchange format |

---

# 📁 Project Structure

```text
weapi
│
├── src/
│   ├── main.c
│   └── application source files
│
├── lib/
│   └── facil/
│       └── facil.io framework source
│
├── Makefile
├── README.md
└── ...
```

---

# 🚀 How to Run

## Requirements

Before running the project, install:

- GCC compiler
- Make
- Linux environment (or WSL on Windows)


## Build

Clone the repository:

```bash
git clone <repository-url>

cd weapi
```

Build the project:

```bash
make
```

---

## Run Server

Execute:

```bash
./weapi
```

The server will start at:

```
http://127.0.0.1:3000
```

---

# 🔌 API Documentation

The API uses HTTP requests and returns JSON responses.

---

# User Routes

## Create User

### POST

```
/user/create
```

Creates a new user.

---

## User Login

### POST

```
/user/login
```

Authenticates a user.

---

## Delete User

### POST

```
/user/delete
```

Removes a user from the system.

---

## Update User

### POST

```
/user/update
```

Updates user information.

---

## Read All Users

### GET

```
/user/readall
```

Returns all registered users.

---

## Read User By ID

### GET

```
/user/read-by-id
```

Returns user information using user ID.

---

## Search User

### POST

```
/user/byid
```

Finds a user based on provided identification data.

---

# 📦 Data Model

User information is stored using a C structure:

```c
struct user {
    char username[25];
    char email[255];
    char password[65];
    int id;
};
```

The current implementation manages users using an internal memory-based storage system.

---

# 🔒 Thread Safety

Since the server can handle multiple requests, shared user data is protected using mutex synchronization.

This prevents race conditions during operations such as:

- Creating users
- Updating user information
- Deleting users

---

# 🧪 Example Response Format

API responses follow a JSON structure:

```json
{
    "status": true,
    "body": {
        "msg": "Operation completed successfully"
    }
}
```

---

# 🎯 Learning Objectives

This project demonstrates practical implementation of:

- Backend development using C
- HTTP server programming
- REST API design
- Low-level memory management
- Concurrent programming concepts
- Working with external libraries

---

# 📜 License

This project is licensed under the **MIT License**.

See the `LICENSE` file for details.

---

# 👤 Author

**Mohammad Mahdi Dehghan**

Undergraduate Student in Electrical Engineering

**Amirkabir University of Technology (Tehran Polytechnic)**

GitHub: [@m-dehghan-eng](https://github.com/m-dehghan-eng)
