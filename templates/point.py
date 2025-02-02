import gdb

class PointPrinter:
    """Pretty Printer for the Point class"""

    def __init__(self, val):
        self.val = val

    def to_string(self):
        x = self.val['x']
        y = self.val['y']

        # Extract std::string value from C++11 ABI
        name_ptr = self.val['name']['_M_dataplus']['_M_p']
        name = name_ptr.string()

        return f"Point(x={x}, y={y}, name='{name}')"

# Function to automatically detect and apply the pretty printer
def lookup_pretty_printer(val):
    if str(val.type) == "Point":
        return PointPrinter(val)
    return None

# Register the pretty printer globally
def register_pretty_printers():
    gdb.pretty_printers.append(lookup_pretty_printer)

# Automatically register when the script is loaded
register_pretty_printers()

# source point.py
# set print pretty on
# g++ point.cpp -g -opoint -O0
# echo "source /path/to/gdb_pretty_printer.py" >> ~/.gdbinit
# echo "set print pretty on" >> ~/.gdbinit
# gdb point
# p p