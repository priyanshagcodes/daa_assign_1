# DAA Assignment - 1

## Group Members
- Priyansh Agarwal(2022A7PS1293H)
- Harsh Jawajiwar (2021B5A72358H)
- Ansh Kalucha(2022A7PS1799H)
- Sarthak Somani(2021B1A70793H)
- Ujjwal Mishra(2021B72479H)

---
Three graph algorithms' C/C++ implementations are included in this repository, along with experimental findings from three datasets.  The `-O3` flag has been used to optimize the algorithms.  The study includes thorough explanations of the algorithms and experimental findings.

---


## Dataset Preparation

1. **Download the Datasets:**

   - [Email-Enron dataset](https://snap.stanford.edu/data/email-Enron.html)
   - [AS-Skitter dataset](https://snap.stanford.edu/data/as-Skitter.html)
   - [Wiki-Vote dataset](https://snap.stanford.edu/data/wiki-Vote.html)

2. **Extract and Organize Files:**

   - Download the ZIP file for each dataset.
   - Unzip each file.
   - Locate the text file containing the graph data from each dataset.

3. **Place and Rename Files:**

   - Move the text files into the `code` directory.
   - Rename the files as follows:
     - Enron dataset → `Email-Enron.txt`
     - Wiki-Vote dataset → `wiki-Vote.txt`
     - AS-Skitter dataset → `as-skitter.txt`

4. **File Format Adjustments:**

   - **Remove Comments:**  
     Remove any comments at the top of each file.
     
   - **Set Header Format:**  
     
     For example, for `wiki-Vote.TXT`, the file should begin with a line similar to:
     ```
     6688    93689
     ```
     followed by the list of edges:
     ```
     320    1412
     153    242
     ...
     ```
    The actual node and edge values are provided in the original TXT file.
---
   ## Compilation Instructions


1. **Modify Dataset File Names in Source Files:**

   - **ELS Algorithm (`els.cpp`):**
     - Open `els.cpp` and go to **line 273**.
     - Change the dataset file name to one of: `as-skitter.txt`, `Email-Enron.txt`, or `wiki-Vote.txt` as needed.
     
   - **Tomita Algorithm (`worst-case.cpp`):**
     - Open `worst-case.cpp` and go to **line 115**.
     - Change the dataset file name to one of: `as-skitter.txt`, `Email-Enron.txt`, or `wiki-Vote.txt` as needed.
     
   - **Chiba Algorithm (`chiba.cpp`):**
     - Open `chiba.cpp` and go to **line 146**.
     - Change the dataset file name to one of: `as-skitter.txt`, `Email-Enron.txt`, or `wiki-Vote.txt` as needed.

2. **Compile the Source Files with g++:**

   Use the `-O3` optimization flag when compiling:

   - Compile `els.cpp`:
     ```bash
     g++ -O3 -o els els.cpp
     els.exe (Windows)
     ./els.exe (Unix)
     ```
   - Compile `worst-case.cpp`:
     ```bash
     g++ -O3 -o worst-case worst-case.cpp
     tomita.exe (Windows)
     ./tomita.exe (Unix)
     ```
   - Compile `chiba.cpp`:
     ```bash
     g++ -O3 -o chiba chiba.cpp
     chiba.exe (Windows)
     ./chiba.exe (Unix)
     ```
---

## Individual Contributions

- **Priyansh Agarwal , Ansh Kalucha & Sarthak Somani**  
  - Developed the worst-case time complexity for generating all maximal cliques and computational experiments by Etsuji Tomitaa, Akira Tanakaa and Haruhisa Takahashi
  - Developed the Arboricity and Subgraph Listing Algorithms by Norishige Chiba and Takao Nishizekvf

- **Ujjwal Mishra & Harsh Jawajiwar**  
  - Developed the Listing All Maximal Cliques in Sparse Graphs in Near-optimal Time Algorithm by David Eppstein, Maarten Loffler, and Darren Strash
  - Created the project report.
