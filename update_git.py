import sys
import subprocess


def update_git(pro_file):
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

        # Hacer un commit con los cambios
        subprocess.run(["git", "add", pro_file])
        subprocess.run(["chmod", "+x", "builds/QTradingview2"])
        subprocess.run(["git", "add", "builds/QTradingview2"])
        subprocess.run(["git", "commit", "-m", "Actualización de versión"])

        # Crear un tag con el nuevo número de versión
        subprocess.run(["git", "tag", f"v{version_major}.{version_minor}.{version_patch}"])
        subprocess.run(["git", "push", "origin", f"v{version_major}.{version_minor}.{version_patch}"])

        print("Commit y tag creados exitosamente")
        return True

    except Exception as e:
        print(f"Error al procesar el archivo {pro_file}: {e}")
        return False

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Uso: python3 update_git.py <ruta_al_archivo_pro>")
        sys.exit(1)

    pro_file = sys.argv[1]
    if not update_git(pro_file):
        sys.exit(1)
