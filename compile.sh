# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
NOC='\033[0m' # no color

# State
ERR="${RED}ERROR ${NOC}"
SUC="${GREEN}SUCCESS ${NOC}"

binDir="./bin"

vertPath="./src/shaders/default.vert"
vertBin="${binDir}/default.vert.spv"

fragPath="./src/shaders/default.frag"
fragBin="${binDir}/default.frag.spv"

compileShaders() {
  glslc $vertPath -o $vertBin # Compile Vertex Shader
  if [ $? -gt 0 ]; then
    echo -e "${ERR}Failed to compile the vertex shader!"
    echo "Is the vertex shader file at $vertPath ?"
  else
    echo -e "${SUC}compiled the vertex shader${NOC}"

    glslc $fragPath -o $fragBin # Compile Fragment Shader
    if [ $? -gt 0 ]; then
      echo -e "${ERR}Failed to compile the fragment shader!"
      echo "Is the fragment shader file at $fragPath ?"
    else
        echo -e "${SUC}compiled the fragment shader${NOC}"
    fi
  fi
}

mkdir ./bin
if [ $? -eq 0 ]; then
  echo "Created ${binDir} Directory"
  compileShaders
elif [ $? -eq 1 ]; then # Directory already exists
  rm ${binDir}/*.spv # remove the binaries inside
  compileShaders
fi