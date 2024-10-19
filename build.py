import os

def read_files_in_directory(directory):
    content = ""
    for root, _, files in os.walk(directory):
        for file in files:
            if file.endswith('.c') or file.endswith('.h'):
                file_path = os.path.join(root, file)
                with open(file_path, 'r', encoding='utf-8') as f:
                    content += f"\n// Content of {file_path}\n"
                    content += f.read() + "\n"
    return content

def write_to_output_file(output_file, content):
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write(content)

def main():
    src_directory = 'src/'        # src dizinini buraya yazın
    include_directory = 'include/' # include dizinini buraya yazın
    output_file = 'combined.c'    # Çıktı dosyası adı

    # Dosyaları oku
    content = read_files_in_directory(include_directory)
    content += read_files_in_directory(src_directory)

    # Çıktıyı dosyaya yaz
    write_to_output_file(output_file, content)
    print(f"Combined C file written to {output_file}")

if __name__ == "__main__":
    main()
