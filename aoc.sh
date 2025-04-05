#!/bin/bash

BASE_URL="https://adventofcode.com"
SESSION_FILE="./aoc.session"

# Read session cookie from file
if [ ! -f "$SESSION_FILE" ]; then
  echo "❌ Session file '$SESSION_FILE' not found."
  echo "   Please create this file with your Advent of Code session cookie:"
  echo "   echo 'your_cookie_here' > $SESSION_FILE"
  exit 1
fi

AOC_SESSION=$(<"$SESSION_FILE")

get_input() {
  local year=$1
  local day=$2
  local output_file="input_day${day}.txt"

  curl "${BASE_URL}/${year}/day/${day}/input" \
    --silent \
    --cookie "session=${AOC_SESSION}" \
    --output "${output_file}"

  echo "✅ Puzzle input saved to ${output_file}"
}

submit_answer() {
  local year=$1
  local day=$2
  local part=$3
  local answer=$4

  local response=$(curl "${BASE_URL}/${year}/day/${day}/answer" \
    --silent \
    --cookie "session=${AOC_SESSION}" \
    --data "level=${part}&answer=${answer}")

  local message=$(echo "$response" | grep -oP '(?<=<article>).*?(?=</article>)' | sed 's/<[^>]*>//g' | head -n 1)

  if echo "$message" | grep -q "That's the right answer"; then
    echo "✅ $message"
  elif echo "$message" | grep -q "That's not the right answer"; then
    echo "❌ $message"
  else
    echo "⚠️ Server response:"
    echo
    echo "$message"
  fi
}

get_description() {
  local year=$1
  local day=$2
  local output_file="description_day${day}.md"

  local description=$(curl "${BASE_URL}/${year}/day/${day}" \
    --silent \
    --cookie "session=${AOC_SESSION}")

  local article=$(echo "${description}" | sed -n '/<article/,/<\/article>/p')

  if [ -z "$article" ]; then
    echo "❌ Failed to fetch description."
    return 1
  fi

  if command -v html2text &> /dev/null; then
    echo "$article" | html2text > "$output_file"
  elif command -v pandoc &> /dev/null; then
    echo "$article" | pandoc -f html -t markdown -o "$output_file"
  else
    echo "❌ Install 'html2text' or 'pandoc' to convert description to Markdown."
    return 1
  fi

  echo "✅ Problem description saved to ${output_file}"
}

# ------------------------------
# Main Argument Parser
# ------------------------------

show_help() {
  echo "Usage:"
  echo "  ./aoc.sh get_input <year> <day>"
  echo "  ./aoc.sh get_description <year> <day>"
  echo "  ./aoc.sh submit_answer <year> <day> <part> <answer>"
  echo
  echo "Ensure './aoc.session' contains your session cookie."
}

get_build_args() {
  # Get the arguments passed to the upstream build process
  build_args=$(ps --no-headers -o args --ppid=$PPID | grep -m1 "./build")
  # Extract positional arguments
  set -- $build_args
  year=$2
  day=$3
  part=$4
}

command=$1
shift || true

case "$command" in
  input)
    get_build_args
    get_input "$year" "$((10#$day))"
    ;;
  description)
    get_build_args
    get_description "$year" "$((10#$day))"
    ;;
  submit)
    get_build_args
    read -r answer_line
    answer=$(echo "$answer_line" | sed 's/^ANSWER: //')
    submit_answer "$year" "$((10#$day))" "$part" "$answer"
    ;;
  help|-h|--help)
    show_help
    ;;
  *)
    echo "❌ Unknown command: $command"
    show_help
    ;;
esac
