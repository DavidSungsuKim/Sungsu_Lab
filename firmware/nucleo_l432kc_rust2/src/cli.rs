
use heapless::{String, Vec};

// Constants
const MAX_CLI_ARGS: usize = 20;
const SIZE_CLI_RX_BUFFER: usize = 64;

// Aliases
pub type FixedStringSlices = Vec<String<SIZE_CLI_RX_BUFFER>, MAX_CLI_ARGS>;
pub type StringCLI = String<SIZE_CLI_RX_BUFFER>;

/**
 * @brief Get the arguments for a CLI command.
 * @details This function gets a character and pushed it into a buffer until a carriage return is received.
 *          When a carriage return is received, the buffer is parsed into a vector of fixed string slices to form the command.
 * 
 * @param ch: the character to be parsed
 * @param buffer: the buffer to store the parsed command
 */
pub fn cli_get_args(ch: u8, buffer: &mut StringCLI) -> Option<FixedStringSlices> {
   if ch != b'\r' {
       // push byte into the string buffer
       buffer.push(ch as char).unwrap();
       None
   } else {
       buffer.push(' ').unwrap(); // to fix a problem with the last number to be treated as a string when parsed.
   
       let mut args: FixedStringSlices = Vec::new();
       let mut start = 0;

       for (index, char) in buffer.char_indices() {
           if char == ' ' {
               if start != index {
                   let slice_str = &buffer[start..index];
                   let mut slice_string: StringCLI = String::new();
                   slice_string.push_str(slice_str).unwrap();
                   let _ = args.push(slice_string);
               }
               start = index + 1;
           }
       }

       if start < buffer.len() {
           let slice_str = &buffer[start..];
           let mut slice_string: StringCLI = String::new();
           slice_string.push_str(slice_str).unwrap();
           let _ = args.push(slice_string);
       }

       buffer.clear();
       Some(args)
   }
}
