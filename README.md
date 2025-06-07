# Zero Knowledge Proof Light

> *A small and simple version using the discrete logarithm*

## Overview

This is a trustless way to prove knowledge of a secret (e.g., a password or choice) without revealing it, based on the discrete logarithm problem.
The prover picks a secret `x` and computes `y = g^x mod p`. 
To prove knowledge of x without revealing it, the prover sends a cryptographic challenge and response that the verifier can check — but which doesn’t reveal x.
All steps can be mathematically verified, but the secret remains hidden.

## 🔧 Requirements

- C compiler (gcc or clang)
- CMake (version 4.0.2 or higher)
- libsodium (cryptographic library)

## 🏗️ Building the Application

### 📦 Installing Dependencies

#### macOS
```bash
brew install libsodium cmake
```

#### Ubuntu/Debian
```bash
apt-get install libsodium-dev cmake
```

#### Fedora/RHEL
```bash
dnf install libsodium-devel cmake
```

### 🔨 Compiling

```bash
# Create a build directory
mkdir build
cd build

# Configure and build
cmake ..
make
```

This will create two executables: `commit` and `verify`.

## How it works

### 🔒 Step 1: Alice commits

One user (Alice) runs the `commit` program:

```bash
./commit
```

When propted, a big prime and a group generator are provided, and Alice has to choose a `x` which will remain secret.
To calculate `C`, Alice must use a random number `r`, which is generated using libsodium.
Both the public key `y` and the commit `C` is now calculated, and Alice has to give Bob (the verifier) these numbers.

### 🤺 Step 2: Bob challenges

Another user (Bob) runs the `commit` program:

```bash
./commit
```

When prompted, the same large prime and group generator as before are provided.
Now Bob must enter `y` and `C`, then select one of these two methods to challenge Alice:

1. Ask Alice for `r`
2. Ask Alice for `s = (x + r) mod (p - 1)`

### 💬 Step 4: Alice answers

Whatever Bob requests, he receives.

### ✅ Step 3: Verification

Bob enters Alice's answer, and the program will tell Bob if Alice tells the truth or not.
If the verification is successful, then Alice has proven to Bob, that she knows a secret `x` without revealing it.

### 🤔 Step 4: Can the Result Be Trusted?

In case Bob chooses the first challenge, where Alice only has to provide `r`, but there is a small chance that Alice doesn't know `x` and chooses a random `r` which happens to be the right `r`.
In this case the verification will be successful, but Alice has lied. To increase the likelihood that Alice cannot lie her way out of it, it is recommended to run the program several times.
With each iteration the program runs, you will reduce the probability of Alice guessing correctly by chance, making it increasingly unlikely that she can deceive the verification.

---

<p align="center">Made by <a href="https://github.com/sigriddalsgard">@sigriddalsgard</a> with ❤️</p>
