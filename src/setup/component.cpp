
#include "setup/component.hpp"

#include "setup/version.hpp"
#include "util/load.hpp"
#include "util/storedenum.hpp"

namespace setup {

namespace {

STORED_FLAGS_MAP(stored_component_flags_0,
	component_entry::Fixed,
	component_entry::Restart,
	component_entry::DisableNoUninstallWarning,
);

// starting with version 3.0.8
STORED_FLAGS_MAP(stored_component_flags_1,
	component_entry::Fixed,
	component_entry::Restart,
	component_entry::DisableNoUninstallWarning,
	component_entry::Exclusive,
);

// starting with version 4.2.3
STORED_FLAGS_MAP(stored_component_flags_2,
	component_entry::Fixed,
	component_entry::Restart,
	component_entry::DisableNoUninstallWarning,
	component_entry::Exclusive,
	component_entry::DontInheritCheck,
);

} // anonymous namespace

void component_entry::load(std::istream & is, const version & version) {
	
	is >> encoded_string(name, version.codepage());
	is >> encoded_string(description, version.codepage());
	is >> encoded_string(types, version.codepage());
	if(version >= INNO_VERSION(4, 0, 1)) {
		is >> encoded_string(languages, version.codepage());
	} else {
		languages.clear();
	}
	if(version >= INNO_VERSION(3, 0, 8)) {
		is >> encoded_string(check, version.codepage());
	} else {
		check.clear();
	}
	
	if(version >= INNO_VERSION(4, 0, 0)) {
		extra_disk_pace_required = load_number<uint64_t>(is);
	} else {
		extra_disk_pace_required = load_number<uint32_t>(is);
	}
	
	if(version >= INNO_VERSION(3, 0, 8)) {
		level = load_number<int32_t>(is);
		used = load_number<uint8_t>(is);
	} else {
		level = 0, used = true;
	}
	
	winver.load(is, version);
	
	if(version >= INNO_VERSION(4, 2, 3)) {
		options = stored_flags<stored_component_flags_2>(is).get();
	} else if(version >= INNO_VERSION(3, 0, 8)) {
		options = stored_flags<stored_component_flags_1>(is).get();
	} else {
		options = stored_flags<stored_component_flags_0>(is).get();
	}
	
	size = (version >= INNO_VERSION(4, 0, 0)) ? load_number<uint64_t>(is) : load_number<uint32_t>(is);
}

} // namespace setup

NAMES(setup::component_entry::flags, "Setup Component Option",
	"fixed",
	"restart",
	"disable no uninstall warning",
	"exclusive",
	"don't inherit check",
)