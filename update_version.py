import sys
import subprocess


def increment_version(pro_file):
    try:
        with open(pro_file, 'r') as file:
            lines = file.readlines()

        new_lines = []
        version_major = None
        version_minor = None
        version_patch = None

        for line in lines:
            if line.startswith('VERSION_MAJOR'):
                parts = line.split('=')
                if len(parts) == 2:
                    version_major = int(parts[1].strip())
                new_lines.append(line)
            elif line.startswith('VERSION_MINOR'):
                parts = line.split('=')
                if len(parts) == 2:
                    version_minor = int(parts[1].strip())
                new_lines.append(line)
            elif line.startswith('VERSION_PATCH'):
                parts = line.split('=')
                if len(parts) == 2:
                    version_patch = int(parts[1].strip())
                new_lines.append(line)
            else:
                new_lines.append(line)

        if version_major is None or version_minor is None or version_patch is None:
            print("No se encontraron todas las versiones (VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH) en el archivo.")
            return False

        # Incrementar la versión
        version_patch += 1
        if version_patch >= 10:
            version_patch = 0
            version_minor += 1
            if version_minor >= 10:
                version_minor = 0
                version_major += 1

        for i in range(len(new_lines)):
            if new_lines[i].startswith('VERSION_MAJOR'):
                new_lines[i] = f'VERSION_MAJOR = {version_major}\n'
            elif new_lines[i].startswith('VERSION_MINOR'):
                new_lines[i] = f'VERSION_MINOR = {version_minor}\n'
            elif new_lines[i].startswith('VERSION_PATCH'):
                new_lines[i] = f'VERSION_PATCH = {version_patch}\n'

        with open(pro_file, 'w') as file:
            file.writelines(new_lines)

        print(f"Incremento de la versión a {version_major}.{version_minor}.{version_patch}")

        return True

    except Exception as e:
        print(f"Error al procesar el archivo {pro_file}: {e}")
        return False

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python3 update_version.py <ruta_al_archivo_pro>")
        sys.exit(1)

    pro_file = sys.argv[1]
    if not increment_version(pro_file):
        sys.exit(1)
