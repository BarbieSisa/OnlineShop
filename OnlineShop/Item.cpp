#include "Item.h"
#include <fstream>
#include <stdexcept>
#include"MyString.h"
MyVector<Item> Item::items;
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
Item::Item(const MyString& name, double price, int quantity,
    const MyString& description, double rating)
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
    char nameBuffer[1024];
    char descriptionBuffer[2048];
    double price, rating;
    int quantity;
    bool available;

    std::cout << "Enter item name: ";
    std::cin.ignore();
    std::cin.getline(nameBuffer, 1024);

    std::cout << "Enter price: ";
    std::cin >> price;

    std::cout << "Enter quantity: ";
    std::cin >> quantity;
    std::cin.ignore();

    std::cout << "Enter description: ";
    std::cin.getline(descriptionBuffer, 2048);

    std::cout << "Enter rating (0.0 - 5.0): ";
    std::cin >> rating;

    std::cout << "Is Available (1 = yes, 0 = no): ";
    std::cin >> available;

    MyString name(nameBuffer);
    MyString description(descriptionBuffer);

    Item item(name, price, quantity, description, rating);
    item.setAvailable(available);

    // Запис във файл с fstream
    std::ofstream outFile("Items.txt", std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Error opening Items.txt.\n";
        return;
    }

    outFile << item.getId() << '\n';
    outFile << item.getName().c_str() << '\n';
    outFile << item.getPrice() << '\n';
    outFile << item.getQuantity() << '\n';
    outFile << item.getDescription().c_str() << '\n';
    outFile << item.getRating() << '\n';
    outFile << item.getAvailable() << '\n';

    outFile.close();

    std::cout << "Item created with ID #" << item.getId() << " and saved.\n";
}
void Item::removeById(int idToRemove) {
    std::ifstream inFile("Items.txt");
    if (!inFile.is_open()) {
        std::cerr << "Cannot open Items.txt for reading.\n";
        return;
    }

    MyVector<MyString> lines;
    char buffer[2048];
    bool found = false;

    while (inFile.getline(buffer, sizeof(buffer))) {
        int currentId = std::atoi(buffer);

        MyVector<MyString> itemBlock;
        itemBlock.push_back(MyString(buffer)); 

        bool incompleteBlock = false;
        for (int i = 0; i < 6; ++i) {
            if (!inFile.getline(buffer, sizeof(buffer))) {
                incompleteBlock = true;
                break;
            }
            itemBlock.push_back(MyString(buffer));
        }

        if (incompleteBlock) break;

        if (currentId == idToRemove) {
            found = true; 
        }
        else {
            for (int i = 0; i < itemBlock.size(); ++i) {
                lines.push_back(itemBlock[i]);
            }
        }
    }
    inFile.close();

    if (!found) {
        std::cout << "Item with ID " << idToRemove << " not found.\n";
        return;
    }

    std::ofstream outFile("Items.txt", std::ios::trunc);
    if (!outFile.is_open()) {
        std::cerr << "Cannot open Items.txt for writing.\n";
        return;
    }

    for (size_t i = 0; i < lines.size(); ++i) {
        outFile << lines[i].c_str() << '\n';
    }

    outFile.close();
    std::cout << "Item with ID " << idToRemove << " successfully removed.\n";
}
void Item::displayAllProducts() {
    std::ifstream inFile("Items.txt");
    if (!inFile.is_open()) {
        std::cerr << "Cannot open Items.txt\n";
        return;
    }

    int id, quantity, available;
    double price, rating;
    char nameBuffer[1024];
    char descriptionBuffer[2048];
    char line[256];

    std::cout << "All Available Products:\n";
    std::cout << "----------------------------------------------\n";

    while (inFile.getline(line, sizeof(line))) {
        id = std::atoi(line);

        inFile.getline(nameBuffer, sizeof(nameBuffer));
        inFile.getline(line, sizeof(line)); price = std::atof(line);
        inFile.getline(line, sizeof(line)); quantity = std::atoi(line);
        inFile.getline(descriptionBuffer, sizeof(descriptionBuffer));
        inFile.getline(line, sizeof(line)); rating = std::atof(line);
        inFile.getline(line, sizeof(line)); available = std::atoi(line);

        if (available) {
            MyString name(nameBuffer);
            MyString description(descriptionBuffer);

            std::cout << "ID: " << id << "\n"
                << "Name: " << name.c_str() << "\n"
                << "Price: " << price << " BGN\n"
                << "Quantity: " << quantity << "\n"
                << "Description: " << description.c_str() << "\n"
                << "Rating: " << rating << "\n"
                << "Available: Yes\n"
                << "----------------------------------------------\n";
        }
    }

    inFile.close();
}
void Item::viewProductById(int searchId) {
    std::ifstream inFile("Items.txt");
    if (!inFile.is_open()) {
        std::cerr << "Cannot open Items.txt\n";
        return;
    }

    char line[256];
    char nameBuffer[1024];
    char descriptionBuffer[2048];

    while (inFile.getline(line, sizeof(line))) {
        int id = toInt(line);

        if (!inFile.getline(nameBuffer, sizeof(nameBuffer))) break;
        if (!inFile.getline(line, sizeof(line))) break;
        double price = toDouble(line);

        if (!inFile.getline(line, sizeof(line))) break;
        int quantity = toInt(line);

        if (!inFile.getline(descriptionBuffer, sizeof(descriptionBuffer))) break;

        if (!inFile.getline(line, sizeof(line))) break;
        double rating = toDouble(line);

        if (!inFile.getline(line, sizeof(line))) break;
        bool available = toInt(line);

        if (id == searchId) {
            MyString name(nameBuffer);
            MyString description(descriptionBuffer);

            std::cout << "Product Info (ID: " << id << ")\n";
            std::cout << "Name: " << name.c_str() << "\n";
            std::cout << "Price: " << price << " BGN\n";
            std::cout << "Quantity: " << quantity << "\n";
            std::cout << "Description: " << description.c_str() << "\n";
            std::cout << "Rating: " << rating << "\n";
            std::cout << "Available: " << (available ? "Yes" : "No") << "\n";

            inFile.close();
            return;
        }
    }

    std::cout << "Product with ID " << searchId << " not found.\n";
    inFile.close();
}
 int Item::toInt(const char* str) {
     int result = 0;
     int sign = 1;
     int i = 0;

     if (str[0] == '-') {
         sign = -1;
         i++;
     }

     for (; str[i] != '\0'; ++i) {
         if (str[i] < '0' || str[i] > '9') break; 
         result = result * 10 + (str[i] - '0');
     }

     return result * sign;
 }
 double Item::toDouble(const char* str) {
     double result = 0.0;
     double frac = 0.0;
     int sign = 1;
     int i = 0;
     bool decimal = false;
     double divisor = 10.0;

     if (str[0] == '-') {
         sign = -1;
         i++;
     }

     for (; str[i] != '\0'; ++i) {
         if (str[i] == '.') {
             decimal = true;
             continue;
         }

         if (str[i] < '0' || str[i] > '9') break;

         if (!decimal) {
             result = result * 10.0 + (str[i] - '0');
         }
         else {
             frac += (str[i] - '0') / divisor;
             divisor *= 10.0;
         }
     }

     return sign * (result + frac);
 }
 void Item::displayByPriceAsc() {
     std::ifstream inFile("Items.txt");
     if (!inFile.is_open()) {
         std::cerr << "Cannot open Items.txt\n";
         return;
     }

     MyVector<Item> localItems;

     char line[256];
     char nameBuffer[1024];
     char descriptionBuffer[2048];

     while (inFile.getline(line, sizeof(line))) {
         int id = toInt(line);

         if (!inFile.getline(nameBuffer, sizeof(nameBuffer))) break;
         if (!inFile.getline(line, sizeof(line))) break;
         double price = toDouble(line);
         if (!inFile.getline(line, sizeof(line))) break;
         int quantity = toInt(line);
         if (!inFile.getline(descriptionBuffer, sizeof(descriptionBuffer))) break;
         if (!inFile.getline(line, sizeof(line))) break;
         double rating = toDouble(line);
         if (!inFile.getline(line, sizeof(line))) break;
         bool available = toInt(line);

         MyString name(nameBuffer);
         MyString description(descriptionBuffer);

         Item item(name, price, quantity, description, rating);
         item.setAvailable(available);
         item.setId(id); // предполагаме, че имаш setId()

         localItems.push_back(item);
     }

     inFile.close();

     // Bubble sort by price (ascending)
     for (size_t i = 0; i < localItems.size(); ++i) {
         for (size_t j = 0; j < localItems.size() - i - 1; ++j) {
             if (localItems[j].getPrice() > localItems[j + 1].getPrice()) {
                 Item temp = localItems[j];
                 localItems[j] = localItems[j + 1];
                 localItems[j + 1] = temp;
             }
         }
     }

     std::cout << "Products Sorted by Price (Low to High):\n";
     for (size_t i = 0; i < localItems.size(); ++i) {
         if (localItems[i].getAvailable()) {
             std::cout << "- " << localItems[i].getId()
                 << " | " << localItems[i].getName().c_str()
                 << " / " << localItems[i].getPrice() << "\n";
         }
     }
 } 
  void Item::displayByAlphabetical() {
     std::ifstream inFile("Items.txt");
     if (!inFile.is_open()) {
         std::cerr << "Cannot open Items.txt\n";
         return;
     }

     MyVector<Item> localItems;

     char line[256];
     char nameBuffer[1024];
     char descriptionBuffer[2048];

     while (inFile.getline(line, sizeof(line))) {
         int id = toInt(line);

         if (!inFile.getline(nameBuffer, sizeof(nameBuffer))) break;
         if (!inFile.getline(line, sizeof(line))) break;
         double price = toDouble(line);
         if (!inFile.getline(line, sizeof(line))) break;
         int quantity = toInt(line);
         if (!inFile.getline(descriptionBuffer, sizeof(descriptionBuffer))) break;
         if (!inFile.getline(line, sizeof(line))) break;
         double rating = toDouble(line);
         if (!inFile.getline(line, sizeof(line))) break;
         bool available = toInt(line);

         MyString name(nameBuffer);
         MyString description(descriptionBuffer);

         Item item(name, price, quantity, description, rating);
         item.setAvailable(available);
         item.setId(id); // Предполага се, че имаш setId()

         localItems.push_back(item);
     }

     inFile.close();

     // Bubble sort по име (азбучен ред)
     for (size_t i = 0; i < localItems.size(); ++i) {
         for (size_t j = 0; j < localItems.size() - i - 1; ++j) {
             if (localItems[j].getName() > localItems[j + 1].getName()) {
                 Item temp = localItems[j];
                 localItems[j] = localItems[j + 1];
                 localItems[j + 1] = temp;
             }
         }
     }

     std::cout << "Products Sorted Alphabetically:\n";
     for (size_t i = 0; i < localItems.size(); ++i) {
         if (localItems[i].getAvailable()) {
             std::cout << "- " << localItems[i].getId()
                 << " | " << localItems[i].getName().c_str() << "\n";
         }
     }
 }
  void Item::displayByPriceDesc() {
      std::ifstream inFile("Items.txt");
      if (!inFile.is_open()) {
          std::cerr << "Cannot open Items.txt\n";
          return;
      }

      MyVector<Item> localItems;

      char line[256];
      char nameBuffer[1024];
      char descriptionBuffer[2048];

      while (inFile.getline(line, sizeof(line))) {
          int id = toInt(line);

          if (!inFile.getline(nameBuffer, sizeof(nameBuffer))) break;
          if (!inFile.getline(line, sizeof(line))) break;
          double price = toDouble(line);
          if (!inFile.getline(line, sizeof(line))) break;
          int quantity = toInt(line);
          if (!inFile.getline(descriptionBuffer, sizeof(descriptionBuffer))) break;
          if (!inFile.getline(line, sizeof(line))) break;
          double rating = toDouble(line);
          if (!inFile.getline(line, sizeof(line))) break;
          bool available = toInt(line);

          MyString name(nameBuffer);
          MyString description(descriptionBuffer);

          Item item(name, price, quantity, description, rating);
          item.setAvailable(available);
          item.setId(id);  // използваме setId() вместо cast hack

          localItems.push_back(item);
      }

      inFile.close();

      // Bubble sort: по цена в низходящ ред
      for (size_t i = 0; i < localItems.size(); ++i) {
          for (size_t j = 0; j < localItems.size() - i - 1; ++j) {
              if (localItems[j].getPrice() < localItems[j + 1].getPrice()) {
                  Item temp = localItems[j];
                  localItems[j] = localItems[j + 1];
                  localItems[j + 1] = temp;
              }
          }
      }

      std::cout << "Products Sorted by Price (High to Low):\n";
      for (size_t i = 0; i < localItems.size(); ++i) {
          if (localItems[i].getAvailable()) {
              std::cout << "- " << localItems[i].getId()
                  << " | " << localItems[i].getName().c_str()
                  << " / " << localItems[i].getPrice() << " BGN\n";
          }
      }
  }
  void Item::displayByRating() {
         std::ifstream inFile("Items.txt");
         if (!inFile.is_open()) {
             std::cerr << "Cannot open Items.txt\n";
             return;
         }

         MyVector<Item> localItems;

         char line[256];
         char nameBuffer[1024];
         char descriptionBuffer[2048];

         while (inFile.getline(line, sizeof(line))) {
             int id = toInt(line);

             if (!inFile.getline(nameBuffer, sizeof(nameBuffer))) break;
             if (!inFile.getline(line, sizeof(line))) break;
             double price = toDouble(line);
             if (!inFile.getline(line, sizeof(line))) break;
             int quantity = toInt(line);
             if (!inFile.getline(descriptionBuffer, sizeof(descriptionBuffer))) break;
             if (!inFile.getline(line, sizeof(line))) break;
             double rating = toDouble(line);
             if (!inFile.getline(line, sizeof(line))) break;
             bool available = toInt(line);

             MyString name(nameBuffer);
             MyString description(descriptionBuffer);

             Item item(name, price, quantity, description, rating);
             item.setAvailable(available);
             item.setId(id);  // безопасно вместо *((int*)&item)

             localItems.push_back(item);
         }

         inFile.close();

         // Bubble sort по рейтинг (низходящ)
         for (size_t i = 0; i < localItems.size(); ++i) {
             for (size_t j = 0; j < localItems.size() - i - 1; ++j) {
                 if (localItems[j].getRating() < localItems[j + 1].getRating()) {
                     Item temp = localItems[j];
                     localItems[j] = localItems[j + 1];
                     localItems[j + 1] = temp;
                 }
             }
         }

         std::cout << "Products Sorted by Rating:\n";
         for (size_t i = 0; i < localItems.size(); ++i) {
             if (localItems[i].getAvailable()) {
                 std::cout << "- " << localItems[i].getId()
                     << " | " << localItems[i].getName().c_str()
                     << " / " << localItems[i].getRating() << "\n";
             }
         }
     }
  Item Item::read(int id) {
      std::ifstream inFile("Items.txt");
      if (!inFile.is_open()) {
          std::cerr << "Cannot open Items.txt\n";
          Item empty;
          empty.setId(-1); 
          return empty;
      }

      char line[256];
      char nameBuffer[1024];
      char descriptionBuffer[2048];

      while (inFile.getline(line, sizeof(line))) {
          int currentId = toInt(line);

          if (!inFile.getline(nameBuffer, sizeof(nameBuffer))) break;
          if (!inFile.getline(line, sizeof(line))) break;
          double price = toDouble(line);
          if (!inFile.getline(line, sizeof(line))) break;
          int quantity = toInt(line);
          if (!inFile.getline(descriptionBuffer, sizeof(descriptionBuffer))) break;
          if (!inFile.getline(line, sizeof(line))) break;
          double rating = toDouble(line);
          if (!inFile.getline(line, sizeof(line))) break;
          bool available = (line[0] == '1');

          if (currentId == id) {
              MyString name(nameBuffer);
              MyString description(descriptionBuffer);
              Item item(name, price, quantity, description, rating);
              item.setAvailable(available);
              item.setId(currentId);
              inFile.close();
              return item;
          }
      }

      inFile.close();

      std::cerr << "Item with ID " << id << " not found.\n";
      Item empty;
      empty.setId(-1); // показва, че не е намерен
      return empty;
  }
  MyString Item::getDescription() const {
      return description;
  }
void Item::setName(const MyString& newName) {
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
    if (!inFile.is_open()) {
        std::cerr << "Cannot open Items.txt\n";
        return;
    }

    MyVector<Item> allItems;

    char line[256];
    char nameBuffer[1024];
    char descriptionBuffer[2048];

    while (inFile.getline(line, sizeof(line))) {
        int currentId = toInt(line);

        if (!inFile.getline(nameBuffer, sizeof(nameBuffer))) break;
        if (!inFile.getline(line, sizeof(line))) break;
        double price = toDouble(line);
        if (!inFile.getline(line, sizeof(line))) break;
        int quantity = toInt(line);
        if (!inFile.getline(descriptionBuffer, sizeof(descriptionBuffer))) break;
        if (!inFile.getline(line, sizeof(line))) break;
        double rating = toDouble(line);
        if (!inFile.getline(line, sizeof(line))) break;
        bool available = toInt(line);

        MyString name(nameBuffer);
        MyString description(descriptionBuffer);

        Item item(name, price, quantity, description, rating);
        item.setId(currentId);
        item.setAvailable(available);

        if (currentId == id) {
            int newQuantity = quantity - quantityToSubtract;
            if (newQuantity < 0) newQuantity = 0;
            item.setQuantity(newQuantity);
            item.setAvailable(newQuantity > 0);
        }

        allItems.push_back(item);
    }

    inFile.close();

    std::ofstream outFile("Items.txt");
    if (!outFile.is_open()) {
        std::cerr << "Cannot open Items.txt for writing\n";
        return;
    }

    for (size_t i = 0; i < allItems.size(); ++i) {
        const Item& item = allItems[i];
        outFile << item.getId() << "\n"
            << item.getName().c_str() << "\n"
            << item.getPrice() << "\n"
            << item.getQuantity() << "\n"
            << item.getDescription().c_str() << "\n"
            << item.getRating() << "\n"
            << item.getAvailable() << "\n";
    }

    outFile.close();
    std::cout << "Quantity updated for item ID #" << id << "\n";
}
void Item::remove(int id) {
    std::ifstream inFile("Items.txt");
    if (!inFile.is_open()) {
        std::cerr << "Cannot open Items.txt\n";
        return;
    }

    MyVector<Item> allItems;
    char line[256];
    char nameBuffer[1024];
    char descriptionBuffer[2048];
    bool found = false;

    while (inFile.getline(line, sizeof(line))) {
        int currentId = toInt(line);

        if (!inFile.getline(nameBuffer, sizeof(nameBuffer))) break;
        if (!inFile.getline(line, sizeof(line))) break;
        double price = toDouble(line);
        if (!inFile.getline(line, sizeof(line))) break;
        int quantity = toInt(line);
        if (!inFile.getline(descriptionBuffer, sizeof(descriptionBuffer))) break;
        if (!inFile.getline(line, sizeof(line))) break;
        double rating = toDouble(line);
        if (!inFile.getline(line, sizeof(line))) break;
        bool available = toInt(line);

        if (currentId == id) {
            found = true; // пропускаме този продукт
            continue;
        }

        MyString name(nameBuffer);
        MyString description(descriptionBuffer);

        Item item(name, price, quantity, description, rating);
        item.setId(currentId);
        item.setAvailable(available);

        allItems.push_back(item);
    }
    inFile.close();

    if (!found) {
        std::cout << "Item with ID " << id << " not found.\n";
        return;
    }

    std::ofstream outFile("Items.txt");
    if (!outFile.is_open()) {
        std::cerr << "Cannot open Items.txt for writing.\n";
        return;
    }

    for (size_t i = 0; i < allItems.size(); ++i) {
        const Item& item = allItems[i];
        outFile << item.getId() << "\n"
            << item.getName().c_str() << "\n"
            << item.getPrice() << "\n"
            << item.getQuantity() << "\n"
            << item.getDescription().c_str() << "\n"
            << item.getRating() << "\n"
            << item.getAvailable() << "\n";
    }

    outFile.close();
    std::cout << "Item with ID " << id << " deleted successfully.\n";
}
void Item::listProducts() {
    std::ifstream inFile("Items.txt");
    if (!inFile.is_open()) {
        std::cout << "Cannot open Items.txt\n";
        return;
    }

    int id, quantity, available;
    double price, rating;
    char line[256];
    char nameBuffer[1024];
    char descriptionBuffer[2048];

    std::cout << "List of Available Products:\n";
    std::cout << "-----------------------------\n";

    while (inFile.getline(line, sizeof(line))) {
        id = toInt(line);

        if (!inFile.getline(nameBuffer, sizeof(nameBuffer))) break;
        if (!inFile.getline(line, sizeof(line))) break;
        price = toDouble(line);
        if (!inFile.getline(line, sizeof(line))) break;
        quantity = toInt(line);
        if (!inFile.getline(descriptionBuffer, sizeof(descriptionBuffer))) break;
        if (!inFile.getline(line, sizeof(line))) break;
        rating = toDouble(line);
        if (!inFile.getline(line, sizeof(line))) break;
        available = toInt(line);

        if (available) {
            MyString name(nameBuffer);
            std::cout << "ID: " << id << " | " << name.c_str() << "\n";
        }
    }

    std::cout << "-----------------------------\n";
    inFile.close();
}
bool Item::getAvailable() const { return quantity > 0; }

int Item::getQuantity() const { return quantity; }
int Item::getId() const { return id; }
double Item::getPrice() const { return price; }
MyString Item::getName() const {
    return name;
}
void Item::updateRating(int productId, double newRating) {
    std::ifstream inFile("Items.txt");
    if (!inFile.is_open()) {
        std::cout << "Cannot open Items.txt\n";
        return;
    }

    MyVector<Item> allItems;
    char line[256];
    char nameBuffer[1024];
    char descriptionBuffer[2048];
    bool found = false;

    while (inFile.getline(line, sizeof(line))) {
        int id = toInt(line);

        if (!inFile.getline(nameBuffer, sizeof(nameBuffer))) break;
        if (!inFile.getline(line, sizeof(line))) break;
        double price = toDouble(line);
        if (!inFile.getline(line, sizeof(line))) break;
        int quantity = toInt(line);
        if (!inFile.getline(descriptionBuffer, sizeof(descriptionBuffer))) break;
        if (!inFile.getline(line, sizeof(line))) break;
        double rating = toDouble(line);
        if (!inFile.getline(line, sizeof(line))) break;
        bool available = toInt(line);

        MyString name(nameBuffer);
        MyString description(descriptionBuffer);

        Item item(name, price, quantity, description, rating);
        item.setAvailable(available);
        item.setId(id);

        if (id == productId) {
            item.setRating(newRating);
            found = true;
        }

        allItems.push_back(item);
    }

    inFile.close();

    if (!found) {
        std::cout << "Product with ID " << productId << " not found.\n";
        return;
    }

    std::ofstream outFile("Items.txt");
    if (!outFile.is_open()) {
        std::cout << "Failed to write to Items.txt\n";
        return;
    }

    for (size_t i = 0; i < allItems.size(); ++i) {
        const Item& item = allItems[i];
        outFile << item.getId() << "\n"
            << item.getName().c_str() << "\n"
            << item.getPrice() << "\n"
            << item.getQuantity() << "\n"
            << item.getDescription().c_str() << "\n"
            << item.getRating() << "\n"
            << item.getAvailable() << "\n";
    }

    outFile.close();

    std::cout << "Rating updated for product ID " << productId << ".\n";
}