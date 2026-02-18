# Multithreaded RLE File Compressor (C + pthread)

## **Overview**

This project implements **Run-Length Encoding (RLE)** compression in **C** using **POSIX Threads (pthread)** for parallel processing.

The program:

- Splits the input file into chunks  
- Uses multiple threads to compress each chunk  
- Merges the compressed output  
- Writes to a compressed file only if compression reduces size  

---

## **Features**

- **Multithreaded compression (4 threads)**
- Efficient RLE encoding
- Binary file support
- Checks if compression is beneficial
- Uses `pthread` for parallel processing

---

## **How It Works**

- The file is divided into 4 equal chunks.
- Each thread compresses its assigned chunk using **Run-Length Encoding**.
- Compression rule:
  - If a character repeats **more than 3 times**, it is encoded as:

    ```
    #<count><character>
    ```

Example:

```
AAAA → #4A
```

- If compression does not reduce file size, output is not generated.

---

## **Project Structure**

```
.
├── compressor.c
└── README.md
```

---

## **Requirements**

- Linux / WSL / macOS
- GCC compiler
- POSIX Threads support

---

## **Compile Instructions (Linux)**

```bash
gcc compressor.c -o compressor -lpthread
```

---

## **Run the Program**

```bash
./compressor -c input_file output_file
```

Example:

```bash
./compressor -c sample.txt compressed.bin
```

---

## **Example Output**

```
File compressed successfully.
Original size: 5000 bytes
Compressed size: 1200 bytes
```

Or:

```
RLE compression not possible. Compressed file is larger than the original.
```

---

## **Threading Model**

- Uses `#define MAX_THREADS 4`
- Each thread:
  - Receives chunk boundaries
  - Compresses independently
  - Stores compressed size
- Main thread:
  - Waits using `pthread_join`
  - Calculates total compressed size

---

## **Limitations**

- Boundary runs across chunks are not merged
- Fixed number of threads (4)
- Only compression implemented (no decompression)

---

## **Concepts Used**

- Run-Length Encoding (RLE)
- POSIX Threads (`pthread_create`, `pthread_join`)
- File I/O (`fopen`, `fread`, `fwrite`)
- Dynamic memory allocation
- File size handling (`stat`)

---

## **Future Improvements**

- Add decompression support
- Merge boundary runs between chunks
- Make thread count dynamic
- Add error handling for file operations

---

## **Author**

Your Name

---

# **Linux Command Flow (Step-by-Step)**

## **1. Save the file**

```bash
nano compressor.c
```

Paste the code, then:

CTRL+O → Enter  
CTRL+X  

---

## **2. Compile**

```bash
gcc compressor.c -o compressor -lpthread
```

---

## **3. Create a test file**

```bash
echo "AAAAABBBBBCCCCCDDDDDEEEEE" > sample.txt
```

---

## **4. Run compression**

```bash
./compressor -c sample.txt compressed.bin
```

---

## **5. Check file sizes**

```bash
ls -lh
```
