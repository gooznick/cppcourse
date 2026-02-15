#include <iostream>
#include <limits>
#include <cmath>
#include <iomanip>
#include <string>

// Helper to calculate precision step (gap to next representable number)
template <typename T>
double get_precision(double value) {
    T val = static_cast<T>(value);
    T next = std::nextafter(val, std::numeric_limits<T>::infinity());
    return static_cast<double>(next - val);
}

void print_row(const std::string& type, double val, double deg_scale = 1.0) {
    double p_float = get_precision<float>(val) * deg_scale;
    double p_double = get_precision<double>(val) * deg_scale;

    std::cout << "| " << std::left << std::setw(10) << type 
              << "| " << std::left << std::setw(15) << val
              << "| " << std::left << std::setw(15) << p_float 
              << "| " << std::left << std::setw(15) << p_double << " |" << std::endl;
}

int main() {
    std::cout << "--- Coordinate System Accuracy (Meters) ---" << std::endl;
    std::cout << "Displaying the 'Step Size' (epsilon) at specific magnitudes.\n" << std::endl;

    std::cout << "| Type      | Value          | Float Step (m)  | Double Step (m) |" << std::endl;
    std::cout << "|-----------|----------------|-----------------|-----------------|" << std::endl;

    // 1. ECEF (Earth Centered, Earth Fixed)
    // ~6,371 km radius. large numbers.
    print_row("ECEF (X)", 6378137.0); 

    // 2. UTM (Universal Transverse Mercator)
    // Easting is usually ~500,000m, Northing can be ~4,000,000m
    print_row("UTM ", 4500000.0);

    // 3. Geographic (Lat/Lon)
    // Degrees. 1 deg approx 111,111 meters.
    // Lat ~32 (Israel), Lon ~34
    // We multiply the degree step by 111,111 to get meters.
    double deg_to_meters = 111111.0;
    print_row("Geo ", 32.0, deg_to_meters);
    
    // 4. Local Tangent Plane (ENU)
    // Small numbers relative to a reference point (e.g. 100 meters away)
    print_row("LLLN", 100.0);

    return 0;
}
