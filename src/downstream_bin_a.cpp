#include "downstream_bin_a/downstream_bin_a.hpp"

#include <yaml.h>

#include <map>
#include <string>

#include "upstream_c/upstream_c.hpp"

namespace downstream_bin_a
{
namespace
{

// Top-level scalar key/value pairs of a YAML mapping. A key whose value is a
// mapping or a sequence is skipped. False if the document does not parse.
bool read_flat_mapping(
  const std::string & document, std::map<std::string, std::string> & fields)
{
  yaml_parser_t parser;
  if (!yaml_parser_initialize(&parser)) {
    return false;
  }
  yaml_parser_set_input_string(
    &parser, reinterpret_cast<const unsigned char *>(document.data()), document.size());

  bool ok = true;
  bool done = false;
  int depth = 0;
  bool have_key = false;
  std::string key;
  while (!done) {
    yaml_event_t event;
    if (!yaml_parser_parse(&parser, &event)) {
      ok = false;
      break;
    }
    switch (event.type) {
      case YAML_MAPPING_START_EVENT:
      case YAML_SEQUENCE_START_EVENT:
        if (depth == 1) {
          have_key = false;  // the value is not a scalar: drop its key
        }
        ++depth;
        break;
      case YAML_MAPPING_END_EVENT:
      case YAML_SEQUENCE_END_EVENT:
        --depth;
        break;
      case YAML_SCALAR_EVENT:
        if (depth == 1) {
          const std::string text(
            reinterpret_cast<const char *>(event.data.scalar.value), event.data.scalar.length);
          if (have_key) {
            fields[key] = text;
            have_key = false;
          } else {
            key = text;
            have_key = true;
          }
        }
        break;
      case YAML_STREAM_END_EVENT:
        done = true;
        break;
      default:
        break;
    }
    yaml_event_delete(&event);
  }
  yaml_parser_delete(&parser);
  return ok;
}

}  // namespace

std::string label_from_yaml(const std::string & document)
{
  std::map<std::string, std::string> fields;
  if (!read_flat_mapping(document, fields) ||
    fields.count("name") == 0 || fields.count("xml") == 0)
  {
    return "invalid";
  }
  return upstream_c::label(fields["name"], fields["xml"]);
}

}  // namespace downstream_bin_a
