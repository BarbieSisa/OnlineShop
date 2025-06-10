#include "Item.h"
#include <fstream>
#include <stdexcept>

std::vector<Item> Item::items;
int Item::nextId = 1;

int Item::findIndexById(int id) {
    for (size_t i = 0; i < items.size(); ++i) {
        if (items[i].id == id) return static_cast<int>(i);
    }
    return -1;
}
void Item::setId(int newId) {
    id = newId;
}


Item::Item(const std::string& name, double price, int quantity,
    const std::string& description, double rating)
    : id(nextId++), name(name), price(price), quantity(quantity),
    description(description), rating(rating), available(quantity > 0) {
}
void Item::setQuantity(int newQuantity) {
    quantity = newQuantity;
}

void Item::setAvailable(bool isAvailable) {
    available = isAvailable;
}
void Item::create() {
    std::string name, description;
    double price, rating;
    int quantity;
    

    std::cout << "Enter item name: ";
    std::getline(std::cin, name);

    std::cout << "Enter price: ";
    std::cin >> price;

    std::cout << "Enter quantity: ";
    std::cin >> quantity;
    std::cin.ignore();

    std::cout << "Enter description: ";
    std::getline(std::cin, description);

    std::cout << "Enter rating (0.0 - 5.0): ";
    std::cin >> rating;
    std::cin.ignore();

    bool available;
    std::cout << "Is Available (1 = yes, 0 = no): ";
    std::cin >> available;

    Item item(name, price, quantity, description, rating);
    item.setAvailable(available);
    items.push_back(item);

    std::ofstream outFile("Items.txt", std::ios::app);
    if (!outFile) {
        std::cerr << "Error opening file.\n";
        return;
    }

    outFile << item.getId() << "\n"
        << item.getName() << "\n"
        << price << "\n"
        << quantity << "\n"
        << description << "\n"
        << rating << "\n"
        << available<< "\n";

    std::cout << "Item created with ID #" << item.getId() << " and saved.\n";
}

Item Item::read(int id) {
    std::ifstream inFile("Items.txt");
    if (!inFile) {
        throw std::runtime_error("Cannot open Items.txt");
    }

    std::string line;
    while (std::getline(inFile, line)) {
        int currentId = std::stoi(line);

        std::string name;
        std::getline(inFile, name);

        std::getline(inFile, line);
        double price = std::stod(line);

        std::getline(inFile, line);
        int quantity = std::stoi(line);

        std::string description;
        std::getline(inFile, description);

        std::getline(inFile, line);
        double rating = std::stod(line);

        std::getline(inFile, line); // ✅ Четем наличността
        bool available = (line == "1" || line == "true");

        if (currentId == id) {
            Item item(name, price, quantity, description, rating);
            item.id = currentId;
            item.setAvailable(available); // не забравяй това
            return item;
        }
    }

    throw std::out_of_range("Item with this ID not found.");
}


void Item::update(int id) {
    try {
        // Load item from file using your read(id)
        Item item = Item::read(id);

        std::cout << "Editing item: " << item.name << "\n";

        std::string newName, newDescription;
        double newPrice, newRating;
        int newQuantity;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // flush newline
        std::cout << "New name: ";
        std::getline(std::cin, newName);

        std::cout << "New price: ";
        std::cin >> newPrice;

        std::cout << "New quantity: ";
        std::cin >> newQuantity;
        std::cin.ignore();

        std::cout << "New description: ";
        std::getline(std::cin, newDescription);

        std::cout << "New rating: ";
        std::cin >> newRating;

        // Update fields
        item.name = newName;
        item.price = newPrice;
        item.quantity = newQuantity;
        item.description = newDescription;
        item.rating = newRating;
        item.available = (newQuantity > 0);

        // Read all items and rewrite updated list
        std::ifstream inFile("Items.txt");
        std::vector<Item> allItems;
        std::string line;

        while (std::getline(inFile, line)) {
            int currentId = std::stoi(line);
            std::string name;
            std::getline(inFile, name);
            std::getline(inFile, line);
            double price = std::stod(line);
            std::getline(inFile, line);
            int quantity = std::stoi(line);
            std::string description;
            std::getline(inFile, description);
            std::getline(inFile, line);
            double rating = std::stod(line);

            Item tempItem(name, price, quantity, description, rating);
            tempItem.id = currentId;
            tempItem.available = quantity > 0;

            // Replace if matching ID
            if (tempItem.id == id) {
                tempItem = item;
            }

            allItems.push_back(tempItem);
        }
        inFile.close();

        // Save back to file
        std::ofstream outFile("Items.txt");
        for (const auto& it : allItems) {
            outFile << it.id << '\n'
                << it.name << '\n'
                << it.price << '\n'
                << it.quantity << '\n'
                << it.description << '\n'
                << it.rating << '\n';
        }
        outFile.close();

        std::cout << "Item updated successfully.\n";

    }
    catch (const std::exception& e) {
        std::cerr << "Update failed: " << e.what() << '\n';
    }
}
std::string Item::getDescription() const {
    return description;
}
void Item::setName(const std::string& newName) {
    name = newName;
}

void Item::setPrice(double newPrice) {
    price = newPrice;
}

double Item::getRating() const {
    return rating;
}
void Item::updateQuantity(int id, int quantityToSubtract) {
    std::ifstream inFile("Items.txt");
    if (!inFile) {
        std::cerr << "❌ Cannot open Items.txt\n";
        return;
    }

    std::vector<Item> allItems;
    std::string line;

    while (std::getline(inFile, line)) {
        int currentId = std::stoi(line);

        std::string name;
        std::getline(inFile, name);

        std::getline(inFile, line);
        double price = std::stod(line);

        std::getline(inFile, line);
        int quantity = std::stoi(line);

        std::string description;
        std::getline(inFile, description);

        std::getline(inFile, line);
        double rating = std::stod(line);

        std::getline(inFile, line);
        bool available = (line == "1" || line == "true");

        Item item(name, price, quantity, description, rating);
        item.setId(currentId);
        item.setAvailable(available);

        // ✅ Update quantity for matching ID
        if (currentId == id) {
            int newQuantity = std::max(0, quantity - quantityToSubtract);
            item.setQuantity(newQuantity);
            item.setAvailable(newQuantity > 0);
        }

        allItems.push_back(item);
    }
    inFile.close();

    // 📝 Rewrite Items.txt
    std::ofstream outFile("Items.txt");
    if (!outFile) {
        std::cerr << "❌ Cannot open Items.txt for writing\n";
        return;
    }

    for (const auto& it : allItems) {
        if (it.getQuantity() >= 0) {
            outFile << it.getId() << '\n'
                << it.getName() << '\n'
                << it.getPrice() << '\n'
                << it.getQuantity() << '\n'
                << it.getDescription() << '\n'
                << it.getRating() << '\n'
                << it.getAvailable() << '\n';
        }
        else std::cerr << "Not enought quantity"; {
            outFile << it.getId() << '\n'
                << it.getName() << '\n'
                << it.getPrice() << '\n'
                << it.getQuantity() << '\n'
                << it.getDescription() << '\n'
                << it.getRating() << '\n'
                << 0 << '\n';
        } break;
    }

    outFile.close();
    std::cout << "✅ Quantity updated for Item ID " << id << "\n";
}



void Item::remove(int id) {
    std::ifstream inFile("Items.txt");
    if (!inFile) {
        throw std::runtime_error("Cannot open Items.txt");
    }

    std::vector<Item> allItems;
    std::string line;
    bool found = false;

    // Read all items
    while (std::getline(inFile, line)) {
        int currentId = std::stoi(line);

        std::string name;
        std::getline(inFile, name);

        std::getline(inFile, line);
        double price = std::stod(line);

        std::getline(inFile, line);
        int quantity = std::stoi(line);

        std::string description;
        std::getline(inFile, description);

        std::getline(inFile, line);
        double rating = std::stod(line);

        if (currentId == id) {
            found = true; // mark for deletion
            continue;     // skip this item
        }

        Item item(name, price, quantity, description, rating);
        item.id = currentId;
        allItems.push_back(item);
    }
    inFile.close();

    if (!found) {
        throw std::out_of_range("Item with this ID not found.");
    }

    // Write remaining items back to file
    std::ofstream outFile("Items.txt");
    if (!outFile) {
        throw std::runtime_error("Cannot open Items.txt for writing.");
    }

    for (const auto& item : allItems) {
        outFile << item.id << '\n'
            << item.name << '\n'
            << item.price << '\n'
            << item.quantity << '\n'
            << item.description << '\n'
            << item.rating << '\n';
    }

    outFile.close();
    std::cout << "Item with ID " << id << " deleted successfully.\n";
}


void Item::listAllItems() {
    std::ifstream inFile("Items.txt");
    if (!inFile) {
        std::cerr << "Cannot open Items.txt\n";
        return;
    }

    std::string line;
    std::vector<int> ids;
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;

        int id = std::stoi(line);
        ids.push_back(id);

        // Skip next 6 lines to move to the next item's ID
        for (int i = 0; i < 6; ++i) std::getline(inFile, line);
    }

    if (ids.empty()) {
        std::cout << "No items available.\n";
        return;
    }

    for (int id : ids) {
        try {
            Item item = Item::read(id);
            item.printDetails();
            std::cout << "-------------------------\n";
        }
        catch (...) {
            std::cerr << "Error reading item with ID " << id << "\n";
        }
    }
}


void Item::printDetails() const {
    std::cout << "Item ID: " << id << "\n"
        << "Name: " << name << "\n"
        << "Price: " << price << "\n"
        << "Quantity: " << quantity << "\n"
        << "Description: " << description << "\n"
        << "Rating: " << rating << "\n"
        << "Available: " << (available ? "Yes" : "No") << "\n";
}

bool Item::getAvailable() const { return quantity > 0; }
int Item::getQuantity() const { return quantity; }
int Item::getId() const { return id; }
double Item::getPrice() const { return price; }
const std::string& Item::getName() const { return name; }
