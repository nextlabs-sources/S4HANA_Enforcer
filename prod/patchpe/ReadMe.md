# Introduce

1. Update PE, injection an extend DLL to a target PE

# To do

1. Need check if current DLL already in PE or not. Do not add same DLLs into a PE twice
2. Injection by insert a new section
    1. Method 1:
        1. Add new section header after current section header list
        2. Extral file
        3. Add new section body at the end of file
    2. Method 2:
        1. Extral file
        2. Move current hold NT header to the end of file.
        3. Add new section header and body.