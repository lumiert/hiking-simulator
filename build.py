#!/usr/bin/env python3
"""
Hiking Simulator - Python Build System for SDL2/OpenGL
Handles compilation and DLL deployment
"""

import os
import sys
import subprocess
import shutil
from pathlib import Path
from typing import List, Optional

class BuildSystem:
    def __init__(self):
        self.project_root = Path(__file__).parent
        self.deps_dir = self.project_root / "dependencies"
        self.sdl2_dir = self.deps_dir / "SDL2-2.28.5" / "x86_64-w64-mingw32"
        self.glew_dir = self.deps_dir / "glew-2.2.0"
        self.glm_dir = self.deps_dir / "glm"
        
        # DLL locations
        self.sdl2_dll = self.sdl2_dir / "bin" / "SDL2.dll"
        self.glew_dll = self.glew_dir / "bin" / "Release" / "x64" / "glew32.dll"
        
        # Compiler flags
        self.compiler = "g++"
        self.std = "-std=c++17"
        self.opt = "-O2"
        self.includes = [
            "-I.",
            f"-I{self.sdl2_dir / 'include'}",
            f"-I{self.glew_dir / 'include'}",
            f"-I{self.glm_dir}"
        ]
        self.libs = [
            f"-L{self.sdl2_dir / 'lib'}",
            f"-L{self.glew_dir / 'lib' / 'Release' / 'x64'}",
            "-lmingw32",
            "-lSDL2main",
            "-lSDL2",
            "-lglew32",
            "-lopengl32",
            "-static-libgcc",
            "-static-libstdc++"
        ]
    
    def verify_dependencies(self) -> bool:
        """Check if all dependencies are available"""
        print("[*] Verifying dependencies...")
        
        deps = {
            "SDL2 includes": self.sdl2_dir / "include",
            "SDL2 libs": self.sdl2_dir / "lib",
            "SDL2 DLL": self.sdl2_dll,
            "GLEW includes": self.glew_dir / "include",
            "GLEW libs": self.glew_dir / "lib" / "Release" / "x64",
            "GLEW DLL": self.glew_dll,
            "GLM": self.glm_dir,
        }
        
        missing = []
        for name, path in deps.items():
            if not path.exists():
                print(f"  [!] Missing: {name} at {path}")
                missing.append(name)
            else:
                print(f"  [OK] Found: {name}")
        
        if missing:
            print(f"\n[ERROR] Missing {len(missing)} dependencies!")
            return False
        
        return True
    
    def compile(self, source: str, output: str, verbose: bool = False) -> bool:
        """Compile a single source file"""
        print(f"\n[*] Compiling {source} -> {output}...")
        
        cmd = [self.compiler, self.std, self.opt, "-Wall"]
        cmd.extend(self.includes)
        cmd.append(source)
        cmd.append("-o")
        cmd.append(output)
        cmd.extend(self.libs)
        
        if verbose:
            print(f"[DEBUG] Command: {' '.join(cmd)}\n")
        
        try:
            result = subprocess.run(
                cmd,
                cwd=self.project_root,
                capture_output=True,
                text=True,
                timeout=60
            )
            
            if result.returncode != 0:
                print(f"[ERROR] Compilation failed!")
                if result.stderr:
                    print(f"STDERR:\n{result.stderr}")
                if result.stdout:
                    print(f"STDOUT:\n{result.stdout}")
                return False
            
            # Check if output file was created
            output_path = self.project_root / output
            if output_path.exists():
                size_mb = output_path.stat().st_size / (1024 * 1024)
                print(f"[✓] Created: {output} ({size_mb:.2f} MB)")
                return True
            else:
                print(f"[ERROR] Output file not created: {output}")
                return False
                
        except subprocess.TimeoutExpired:
            print(f"[ERROR] Compilation timeout!")
            return False
        except Exception as e:
            print(f"[ERROR] Compilation error: {e}")
            return False
    
    def copy_dlls(self) -> bool:
        """Copy necessary DLLs to project root"""
        print(f"\n[*] Deploying DLLs...")
        
        dlls = [
            (self.sdl2_dll, "SDL2.dll"),
            (self.glew_dll, "glew32.dll"),
        ]
        
        for src, name in dlls:
            if not src.exists():
                print(f"  [!] DLL not found: {src}")
                continue
            
            dst = self.project_root / name
            try:
                shutil.copy2(src, dst)
                print(f"  [✓] Copied {name}")
            except Exception as e:
                print(f"  [!] Failed to copy {name}: {e}")
                return False
        
        return True
    
    def build_all(self, verbose: bool = False) -> bool:
        """Build all targets"""
        print("\n" + "="*60)
        print(" HIKING SIMULATOR BUILD SYSTEM (Python)")
        print("="*60)
        
        if not self.verify_dependencies():
            return False
        
        if not self.copy_dlls():
            print("[WARNING] DLL deployment had issues, continuing anyway...")
        
        targets = [
            ("main.cpp", "hiking.exe"),
            ("main_shaders.cpp", "shaders.exe"),
        ]
        
        success = True
        for source, output in targets:
            if not self.compile(source, output, verbose):
                success = False
        
        if success:
            print("\n" + "="*60)
            print(" BUILD SUCCESSFUL!")
            print("="*60)
            print("\nRun executables:")
            print("  > hiking.exe         (main game)")
            print("  > shaders.exe        (shader development tool)")
            print()
        
        return success
    
    def clean(self) -> bool:
        """Clean build artifacts"""
        print(f"\n[*] Cleaning build files...")
        
        targets = ["hiking.exe", "shaders.exe", "SDL2.dll", "glew32.dll"]
        for target in targets:
            path = self.project_root / target
            if path.exists():
                try:
                    path.unlink()
                    print(f"  [✓] Removed {target}")
                except Exception as e:
                    print(f"  [!] Failed to remove {target}: {e}")
        
        print("[✓] Clean complete")
        return True
    
    def rebuild(self, verbose: bool = False) -> bool:
        """Clean and rebuild"""
        self.clean()
        return self.build_all(verbose)

def main():
    import argparse
    
    parser = argparse.ArgumentParser(
        description="Hiking Simulator Build System",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  python build.py                 Build all targets
  python build.py --clean         Clean build files
  python build.py --rebuild       Clean and rebuild
  python build.py --verbose       Build with verbose output
        """
    )
    
    parser.add_argument("--clean", action="store_true", help="Clean build artifacts")
    parser.add_argument("--rebuild", action="store_true", help="Clean and rebuild")
    parser.add_argument("--verbose", action="store_true", help="Verbose output")
    parser.add_argument("--check", action="store_true", help="Check dependencies only")
    
    args = parser.parse_args()
    
    builder = BuildSystem()
    
    if args.check:
        success = builder.verify_dependencies()
        return 0 if success else 1
    
    if args.clean:
        return 0 if builder.clean() else 1
    
    if args.rebuild:
        return 0 if builder.rebuild(args.verbose) else 1
    
    return 0 if builder.build_all(args.verbose) else 1

if __name__ == "__main__":
    sys.exit(main())
