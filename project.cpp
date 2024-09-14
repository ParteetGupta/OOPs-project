 #include <iostream>
#include <vector>
#include <map>
#include <string>

using namespace std;

class Product {
public:
    int id;
    string name;
    double price;
    int stock;

    Product(int i, const string& n, double p, int s) : id(i), name(n), price(p), stock(s) {}
};

class Order {
private:
    map<int, int> orderDetails; // Product ID and Quantity

public:
    void addProductToOrder(int productId, int quantity) {
        orderDetails[productId] += quantity;
    }

    map<int, int> getOrderDetails() const {
        return orderDetails;
    }
};

class Inventory {
private:
    vector<Product> products;
    map<int, double> sales;

public:
    void addProduct(const Product& product) {
        products.push_back(product);
    }

    void displayProducts() const {
        cout << "Product List:\n";
        for (size_t i = 0; i < products.size(); ++i) {
            const Product& product = products[i];
            cout << "ID: " << product.id << "\nName: " << product.name << "\nPrice: Rs " << product.price
                 << "\nStock: " << product.stock << "\n\n";
        }
    }

    const Product* findProductById(int productId) const {
        for (size_t i = 0; i < products.size(); ++i) {
            if (products[i].id == productId) {
                return &products[i];
            }
        }
        return nullptr;
    }

    bool sellProduct(Order& order) {
        map<int, int> orderDetails = order.getOrderDetails();
        for (const auto& entry : orderDetails) {
            const int productId = entry.first;
            const int quantity = entry.second;
            const Product* product = findProductById(productId);
            if (product && product->stock >= quantity) {
                sales[productId] += product->price * quantity;
                for (size_t i = 0; i < products.size(); ++i) {
                    if (products[i].id == productId) {
                        products[i].stock -= quantity;
                        break;
                    }
                }
            } else {
                cout << "Product with ID " << productId << " not found or insufficient stock!" << endl;
                return false;
            }
        }
        cout << "Order processed successfully!" << endl;
        return true;
    }

    void generateInvoice(const Order& order) const {
        cout << "-------------------------\n";
        cout << "Invoice for the order:\n";
        cout << "-------------------------\n";
        double totalAmount = 0.0;
        map<int, int> orderDetails = order.getOrderDetails();
        for (const auto& entry : orderDetails) {
            const int productId = entry.first;
            const int quantity = entry.second;
            const Product* product = findProductById(productId);
            if (product) {
                cout << "Product: " << product->name<<endl<< "Quantity: " << quantity<<endl << "Price: Rs_" << product->price << endl;
                totalAmount += product->price * quantity;
            }
        }
        cout << "Total Amount: Rs_" << totalAmount << endl;
    }

    void generateSalesReport() const {
        cout << "-------------------------\n";
        cout << "Sales Report:\n";
        cout << "-------------------------\n";
        for (const auto& entry : sales) {
            const int productId = entry.first;
            const double totalSales = entry.second;
            const Product* product = findProductById(productId);
            if (product) {
                cout << "Product: " << product->name<<endl<< "Total Sales: Rs_" << totalSales << endl;
            }
        }
    }

    void restockProduct(int productId, int quantity) {
        for (size_t i = 0; i < products.size(); ++i) {
            if (products[i].id == productId) {
                products[i].stock += quantity;
                cout << "Restocked " << quantity << " units of " << products[i].name << endl;
                return;
            }
        }
        cout << "Product with ID " << productId << " not found!" << endl;
    }

    void purchaseProduct() {
        char continueBuying = 'y';

        while (tolower(continueBuying) == 'y') {
            int productId, quantity;
            cout << "Enter the ID of the product you want to purchase: ";
            cin >> productId;

            const Product* product = findProductById(productId);
            if (product) {
                cout << "Product found: " << product->name<<endl << "Available stock: " << product->stock << endl;
                cout << "Enter the quantity you want to purchase: ";
                cin >> quantity;

                if (quantity > 0 && quantity <= product->stock) {
                    Order order;
                    order.addProductToOrder(productId, quantity);
                    sellProduct(order);
                    generateInvoice(order);
                } else {
                    cout << "Invalid quantity or insufficient stock!" << endl;
                }
            } else {
                cout << "Product with ID " << productId << " not found!" << endl;
            }

            cout << "Do you want to buy another product? (y/n): ";
            cin >> continueBuying;
        }
    }
};

int main() {
    Inventory inventory;

    // Adding products to inventory
    Product prod1(1, "Product A", 1099, 50);
    Product prod2(2, "Product B", 549, 100);

    inventory.addProduct(prod1);
    inventory.addProduct(prod2);

    // Display available products
    inventory.displayProducts();

    // User-specific purchase
    inventory.purchaseProduct();

    // Generate sales report after purchase
    inventory.generateSalesReport();

    // Restock products
    inventory.restockProduct(1, 10);

    // Display updated product list after restocking
    inventory.displayProducts();

    return 0;
}
