#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

// Class representing a product in the catalogue
class Product {
public:
    string id;
    string name;
    float price;
    string category;

    // Default constructor
    Product() : id(""), name(""), price(0.0), category("") {}

    // Parameterized constructor
    Product(string id, string name, float price, string category) 
        : id(id), name(name), price(price), category(category) {}
};

// Class representing the shopping cart
class Cart {
public:
    unordered_map<string, int> items; // Stores product ID and quantity
    float totalBeforeDiscount = 0;

    // Add product to cart
    void addToCart(Product& product, int quantity) {
        items[product.id] += quantity;
        totalBeforeDiscount += product.price * quantity;
        cout << quantity << " " << product.name << "(s) added to cart." << endl;
    }

    // Remove product from cart
    void removeFromCart(Product& product, int quantity) {
        if (items.find(product.id) != items.end()) {
            if (items[product.id] <= quantity) {
                totalBeforeDiscount -= product.price * items[product.id];
                items.erase(product.id);
                cout << product.name << " removed from cart." << endl;
            } else {
                items[product.id] -= quantity;
                totalBeforeDiscount -= product.price * quantity;
                cout << quantity << " " << product.name << "(s) removed from cart." << endl;
            }
        } else {
            cout << product.name << " is not in the cart." << endl;
        }
    }

    // View cart details
    void viewCart(const unordered_map<string, Product>& productCatalog) {
        if (items.empty()) {
            cout << "Your cart is empty!" << endl;
            return;
        }
        cout << "Your Cart:" << endl;
        int i = 1;
        for (auto& item : items) {
            Product product = productCatalog.at(item.first);
            cout <<i<<". "<< product.name << " - Quantity: " << item.second 
                 << ", Price: " << product.price 
                 << " USD, Total: " << product.price * item.second << " USD" << endl;
            i++;
        }
        cout << "Total (before discounts): " << totalBeforeDiscount << " USD" << endl;
    }
};

// Class representing discount management
class Discount {
public:
    // Apply available discounts on the cart
    float applyDiscounts(Cart& cart, const unordered_map<string, Product>& productCatalog) {
        float discount = 0.0;
        for (auto& item : cart.items) {
            Product product = productCatalog.at(item.first);

            // 10% off Electronics
            if (product.category == "Electronics") {
                discount += (product.price * item.second) * 0.10;
            }

            // Buy 1 Get 1 Free for Fashion
            if (product.category == "Fashion" && item.second >= 2) {
                discount += product.price * (item.second / 2);
            }
        }
        return discount;
    }

    // List available discounts
    void listDiscounts() {
        cout << "Available Discounts:\n";
        cout << "1. Buy 1 Get 1 Free on Fashion items\n";
        cout << "2. 10% Off on Electronics\n";
    }
};

// Class representing currency conversion
class CurrencyConverter {
public:
    float convertToCurrency(float totalUSD, string currency) {
        if (currency == "EUR") {
            return totalUSD * 0.85;
        } else if (currency == "GBP") {
            return totalUSD * 0.75;
        }
        return totalUSD; // Default to USD if no conversion needed
    }
};

void displayMenu() {
    cout << "\n--- E-commerce Cart ---\n";
    cout << "1. Add item to cart\n";
    cout << "2. Remove item from cart\n";
    cout << "3. View cart\n";
    cout << "4. List available discounts\n";
    cout << "5. Checkout\n";
    cout << "6. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    // Initialize product catalog
    unordered_map<string, Product> productCatalog;
    productCatalog["P001"] = Product("P001", "Laptop", 1000.00, "Electronics");
    productCatalog["P002"] = Product("P002", "Phone", 500.00, "Electronics");
    productCatalog["P003"] = Product("P003", "T-Shirt", 20.00, "Fashion");

    Cart cart;
    Discount discountSystem;
    CurrencyConverter currencyConverter;
    
    int choice;
    string productId;
    int quantity;
    
    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1: { // Add item to cart
                cout << "Enter product ID: ";
                cin >> productId;
                cout << "Enter quantity: ";
                cin >> quantity;
                
                if (productCatalog.find(productId) != productCatalog.end()) {
                    cart.addToCart(productCatalog[productId], quantity);
                } else {
                    cout << "Product not found." << endl; //if product is not available in the catalog
                }
                break;
            }
            case 2: { // Remove item from cart
                cout << "Enter product ID: ";
                cin >> productId;
                cout << "Enter quantity to remove: ";
                cin >> quantity;

                if (productCatalog.find(productId) != productCatalog.end()) {
                    cart.removeFromCart(productCatalog[productId], quantity);
                } else {
                    cout << "Product not found." << endl;  // if product is not in the cart
                }
                break;
            }
            case 3: { // View cart
                cart.viewCart(productCatalog);
                break;
            }
            case 4: { // List discounts
                discountSystem.listDiscounts();
                break;
            }
            case 5: { // Checkout and currency selection(if)
                float discountAmount = discountSystem.applyDiscounts(cart, productCatalog);
                float totalAfterDiscount = cart.totalBeforeDiscount - discountAmount;
                cout<<"Applying discounts..."<<endl;

                cout << "Final Total in USD (after discounts): " << totalAfterDiscount << " USD" << endl;

                cout << "Would you like to view it in a different currency? (yes/no): "; // Asking for currency information...
                string response;
                cin >> response;

                if (response == "yes") {
                    cout << "Available Currencies: EUR, GBP\n";
                    cout << "Enter currency: ";
                    string currency;
                    cin >> currency;

                    float convertedTotal = currencyConverter.convertToCurrency(totalAfterDiscount, currency);
                    cout << "Final Total in " << currency << ": " << convertedTotal << endl;
                }
                break;
            }
            case 6: { // Exit
                cout << "Exiting the system..." << endl;
                return 0;
            }
            default: { // default, for wrong case selection
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    return 0;
}
