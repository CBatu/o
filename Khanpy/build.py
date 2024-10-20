import os

def collect_python_files(directory):
    collected_files = set()
    python_code = ""

    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.endswith(".py"):
                file_path = os.path.join(root, file)
                if file_path not in collected_files:
                    collected_files.add(file_path)
                    with open(file_path, 'r', encoding='utf-8') as f:
                        python_code += f"# File: {file_path}\n"
                        python_code += f.read() + "\n\n"
    
    return python_code

# Örnek kullanım:
directory = '.'  # Klasör yolunu buraya girin
all_python_code = collect_python_files(directory)

# İsterseniz sonuçları bir dosyaya yazdırabilirsiniz
with open("output.py", 'w', encoding='utf-8') as output_file:
    output_file.write(all_python_code)
