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
void Item::setRating(double newRating) {
    rating = newRating;
}
void Item::create() {
    std::string name, description;
    double price, rating;
    int quantity;
    bool available;

    std::cout << "Enter item name: ";
    std::cin.ignore(); 
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

    std::cout << "Is Available (1 = yes, 0 = no): ";
    std::cin >> available;

    Item item(name, price, quantity, description, rating);
    item.setAvailable(available);

    items.push_back(item);  

    std::ofstream outFile("Items.txt", std::ios::app);
    if (!outFile) {
        std::cerr << "Error opening Items.txt.\n";
        return;
    }

    outFile << item.getId() << "\n"
        << item.getName() << "\n"
        << item.getPrice() << "\n"
        << item.getQuantity() << "\n"
        << item.getDescription() << "\n"
        << item.getRating() << "\n"
        << item.getAvailable() << "\n";

    outFile.close();
    std::cout << "Item created with ID #" << item.getId() << " and saved.\n";
}
void Item::removeById(int idToRemove) {
    std::ifstream inFile("Items.txt");
    if (!inFile) {
        std::cerr << "❌ Cannot open Items.txt for reading.\n";
        return;
    }

    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    while (std::getline(inFile, line)) {
        int currentId = std::stoi(line);
        std::vector<std::string> itemBlock;
        itemBlock.push_back(line);  // ID

        for (int i = 0; i < 6; ++i) {
            if (!std::getline(inFile, line)) break;
            itemBlock.push_back(line);
        }

        if (currentId == idToRemove) {
            found = true;  // Skip this item
        }
        else {
            lines.insert(lines.end(), itemBlock.begin(), itemBlock.end());
        }
    }
    inFile.close();

    if (!found) {
        std::cout << "⚠️ Item with ID " << idToRemove << " not found.\n";
        return;
    }

    std::ofstream outFile("Items.txt");
    if (!outFile) {
        std::cerr << "❌ Cannot open Items.txt for writing.\n";
        return;
    }

    for (const std::string& l : lines) {
        outFile << l << '\n';
    }

    outFile.close();
    std::cout << "✅ Item with ID " << idToRemove << " successfully removed.\n";
}

 void Item::displayAllProducts() {
    std::ifstream inFile("Items.txt");
    if (!inFile) {
        std::cerr << "❌ Cannot open Items.txt\n";
        return;
    }

    int id, quantity, available;
    double price, rating;
    std::string name, description;
    std::string line;

    std::cout << "📦 All Available Products:\n";
    std::cout << "----------------------------------------------\n";

    while (std::getline(inFile, line)) {
        id = std::stoi(line);

        std::getline(inFile, name);
        std::getline(inFile, line); price = std::stod(line);
        std::getline(inFile, line); quantity = std::stoi(line);
        std::getline(inFile, description);
        std::getline(inFile, line); rating = std::stod(line);
        std::getline(inFile, line); available = std::stoi(line);

        if (available) {
            std::cout << "🆔 ID: " << id << "\n"
                << "🛍️  Name: " << name << "\n"
                << "💰 Price: " << price << " BGN\n"
                << "📦 Quantity: " << quantity << "\n"
                << "📝 Description: " << description << "\n"
                << "⭐ Rating: " << rating << "\n"
                << "✅ Available: Yes\n"
                << "----------------------------------------------\n";
        }
    }

    inFile.close();
}
 void Item::viewProductById(int searchId) {
     std::ifstream inFile("Items.txt");
     if (!inFile) {
         std::cerr << "Cannot open Items.txt\n";
         return;
     }

     std::string line;
     while (std::getline(inFile, line)) {
         int id = std::stoi(line);

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
         bool available = std::stoi(line);

         if (id == searchId) {
             std::cout << "📦 Product Info (ID: " << id << ")\n";
             std::cout << "Name: " << name << "\n";
             std::cout << "Price: " << price << " BGN\n";
             std::cout << "Quantity: " << quantity << "\n";
             std::cout << "Description: " << description << "\n";
             std::cout << "Rating: " << rating << "\n";
             std::cout << "Available: " << (available ? "Yes" : "No") << "\n";
             inFile.close();
             return;
         }
     }

     std::cout << "❌ Product with ID " << searchId << " not found.\n";
     inFile.close();
 }
 void Item::displayByPriceAsc() {
     std::ifstream inFile("Items.txt");
     if (!inFile) {
         std::cerr << "❌ Cannot open Items.txt\n";
         return;
     }

     std::vector<Item> localItems;
     std::string line;

     while (std::getline(inFile, line)) {
         int id = std::stoi(line);

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
         bool available = std::stoi(line);

         Item item(name, price, quantity, description, rating);
         item.setAvailable(available);
         *((int*)&item) = id; // set ID directly if you don't have setId()

         localItems.push_back(item);
     }

     inFile.close();

     // Bubble sort by price ascending
     for (size_t i = 0; i < localItems.size(); ++i) {
         for (size_t j = 0; j < localItems.size() - i - 1; ++j) {
             if (localItems[j].getPrice() > localItems[j + 1].getPrice()) {
                 std::swap(localItems[j], localItems[j + 1]);
             }
         }
     }

     std::cout << "Products Sorted by Price (Low to High):\n";
     for (const auto& item : localItems) {
         if (item.getAvailable()) {
             std::cout << "-" << item.getId() << " | " << item.getName() <<"/ "<< item.getPrice() << "\n";
         }
     }
 }
 void Item::displayByAlphabetical() {
     std::ifstream inFile("Items.txt");
     if (!inFile) {
         std::cerr << "Cannot open Items.txt\n";
         return;
     }

     std::vector<Item> localItems;
     std::string line;

     while (std::getline(inFile, line)) {
         int id = std::stoi(line);

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
         bool available = std::stoi(line);

         Item item(name, price, quantity, description, rating);
         item.setAvailable(available);
         *((int*)&item) = id;

         localItems.push_back(item);
     }

     inFile.close();

     // ✅ Bubble sort по име (азбучен ред)
     for (size_t i = 0; i < localItems.size(); ++i) {
         for (size_t j = 0; j < localItems.size() - i - 1; ++j) {
             if (localItems[j].getName() > localItems[j + 1].getName()) {
                 std::swap(localItems[j], localItems[j + 1]);
             }
         }
     }

     std::cout << "Products Sorted Alphabetically:\n";
     for (const auto& item : localItems) {
         if (item.getAvailable()) {
             std::cout << "- " << item.getId() << " | " << item.getName() << "\n";
         }
     }
 }
 void Item::displayByPriceDesc() {
     std::ifstream inFile("Items.txt");
     if (!inFile) {
         std::cerr << "Cannot open Items.txt\n";
         return;
     }

     std::vector<Item> localItems;
     std::string line;

     while (std::getline(inFile, line)) {
         int id = std::stoi(line);

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
         bool available = std::stoi(line);

         Item item(name, price, quantity, description, rating);
         item.setAvailable(available);
         *((int*)&item) = id;

         localItems.push_back(item);
     }

     inFile.close();

     // ✅ Bubble sort по цена в низходящ ред
     for (size_t i = 0; i < localItems.size(); ++i) {
         for (size_t j = 0; j < localItems.size() - i - 1; ++j) {
             if (localItems[j].getPrice() < localItems[j + 1].getPrice()) {
                 std::swap(localItems[j], localItems[j + 1]);
             }
         }
     }

     std::cout << "Products Sorted by Price (High to Low):\n";
     for (const auto& item : localItems) {
         if (item.getAvailable()) {
             std::cout << "- " << item.getId() << " | " << item.getName()
                 << " / " << item.getPrice() << " BGN\n";
         }
     }
 }
 void Item::displayByRating() {
     std::ifstream inFile("Items.txt");
     if (!inFile) {
         std::cerr << "Cannot open Items.txt\n";
         return;
     }

     std::vector<Item> localItems;
     std::string line;

     while (std::getline(inFile, line)) {
         int id = std::stoi(line);

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
         bool available = std::stoi(line);

         Item item(name, price, quantity, description, rating);
         item.setAvailable(available);
         *((int*)&item) = id; // set ID directly if no setId()

         localItems.push_back(item);
     }

     inFile.close();

     // ✅ Bubble sort by rating (descending)
     for (size_t i = 0; i < localItems.size(); ++i) {
         for (size_t j = 0; j < localItems.size() - i - 1; ++j) {
             if (localItems[j].getRating() < localItems[j + 1].getRating()) {
                 std::swap(localItems[j], localItems[j + 1]);
             }
         }
     }

     std::cout << "Products Sorted by Rating:\n";
     for (const auto& item : localItems) {
         if (item.getAvailable()) {
             std::cout << "- " << item.getId() << " | " << item.getName() << " / " << item.getRating() << "\n";
         }
     }
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
        std::getline(inFile, line); double price = std::stod(line);
        std::getline(inFile, line); int quantity = std::stoi(line);
        std::string description;
        std::getline(inFile, description);
        std::getline(inFile, line); double rating = std::stod(line);
        std::getline(inFile, line); bool available = (line == "1" || line == "true");

        Item item(name, price, quantity, description, rating);
        item.setId(currentId);
        item.setAvailable(available);

        if (currentId == id) {
            int newQuantity = std::max(0, quantity - quantityToSubtract);
            item.setQuantity(newQuantity);
            item.setAvailable(newQuantity > 0);
        }

        allItems.push_back(item);
    }
    inFile.close();

    std::ofstream outFile("Items.txt");
    if (!outFile) {
        std::cerr << "❌ Cannot open Items.txt for writing\n";
        return;
    }

    for (const auto& item : allItems) {
        outFile << item.getId() << "\n"
            << item.getName() << "\n"
            << item.getPrice() << "\n"
            << item.getQuantity() << "\n"
            << item.getDescription() << "\n"
            << item.getRating() << "\n"
            << item.getAvailable() << "\n";
    }

    outFile.close();
    std::cout << "✅ Quantity updated for item ID #" << id << "\n";
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
void Item::listProducts() {
    std::ifstream inFile("Items.txt");
    if (!inFile) {
        std::cerr << "❌ Cannot open Items.txt\n";
        return;
    }

    int id, quantity, available;
    double price, rating;
    std::string name, description;
    std::string line;

    std::cout << "📋 Списък с налични артикули:\n";
    std::cout << "-----------------------------\n";

    while (std::getline(inFile, line)) {
        id = std::stoi(line);

        std::getline(inFile, name);
        std::getline(inFile, line); price = std::stod(line);
        std::getline(inFile, line); quantity = std::stoi(line);
        std::getline(inFile, description);
        std::getline(inFile, line); rating = std::stod(line);
        std::getline(inFile, line); available = std::stoi(line);

        if (available) {
            std::cout << "🆔 " << id << " | " << name << "\n";
        }
    }

    std::cout << "-----------------------------\n";
    inFile.close();
}
bool Item::getAvailable() const { return quantity > 0; }
int Item::getQuantity() const { return quantity; }
int Item::getId() const { return id; }
double Item::getPrice() const { return price; }
const std::string& Item::getName() const { return name; }
void Item::updateRating(int productId, double newRating) {
    std::ifstream inFile("Items.txt");
    if (!inFile) {
        std::cerr << "❌ Cannot open Items.txt\n";
        return;
    }

    std::vector<Item> allItems;
    std::string line;
    bool found = false;

    while (std::getline(inFile, line)) {
        int id = std::stoi(line);

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
        bool available = std::stoi(line);

        Item item(name, price, quantity, description, rating);
        item.setAvailable(available);
        *((int*)&item) = id;

        if (id == productId) {
            item.setRating(newRating); // трябва да имаш тази функция
            found = true;
        }

        allItems.push_back(item);
    }

    inFile.close();

    if (!found) {
        std::cerr << "❌ Product with ID " << productId << " not found.\n";
        return;
    }

    std::ofstream outFile("Items.txt");
    if (!outFile) {
        std::cerr << "❌ Failed to write to Items.txt\n";
        return;
    }

    for (const auto& item : allItems) {
        outFile << item.getId() << "\n"
            << item.getName() << "\n"
            << item.getPrice() << "\n"
            << item.getQuantity() << "\n"
            << item.getDescription() << "\n"
            << item.getRating() << "\n"
            << item.getAvailable() << "\n";
    }

    std::cout << "✅ Rating updated for product ID " << productId << ".\n";
}
 