#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMessageBox>
#include <QFormLayout>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QRandomGenerator>
#include <QPen>
#include <QBrush>

#include "../include/BinaryTree.hpp"
#include "../include/Exceptions.hpp"
#include "../include/Traversal.hpp"
#include "../src/BinaryTree.tpp"

#include <sstream>
#include <iostream>

class TreeNodeItem : public QGraphicsEllipseItem {
public:
    TreeNodeItem(const QString& text, qreal x, qreal y) {
        setRect(0, 0, 40, 40);
        setPos(x - 20, y - 20);
        setBrush(Qt::white);
        setPen(QPen(Qt::black, 2));

        auto* textItem = new QGraphicsTextItem(text, this);
        QRectF bounds = textItem->boundingRect();
        textItem->setPos(20 - bounds.width()/2, 20 - bounds.height()/2);
    }
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow() {
        setWindowTitle("ЛР-1: Бинарное дерево (Вариант 1)");
        resize(1000, 700);

        auto* centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        auto* mainLayout = new QHBoxLayout(centralWidget);

        auto* controlLayout = new QVBoxLayout();
        auto* formLayout = new QFormLayout();

        inputValue = new QLineEdit();
        inputValue->setPlaceholderText("Например: 5");
        btnInsert = new QPushButton("Вставить значение");
        btnGenerate = new QPushButton("Сгенерировать случайное дерево (20)");
        
        formLayout->addRow(new QLabel("<b>Элемент:</b>"), inputValue);
        formLayout->addRow(btnInsert);
        formLayout->addRow(btnGenerate);

        comboTraversal = new QComboBox();
        comboTraversal->addItems({"KLP", "KPL", "LPK", "LKP", "PLK", "PKL"});
        btnTraverse = new QPushButton("Выполнить обход");

        formLayout->addRow(new QLabel("<b>Тип обхода:</b>"), comboTraversal);
        formLayout->addRow(btnTraverse);

        btnMap = new QPushButton("Map (x * 2)");
        btnWhere = new QPushButton("Where (x > 5)");
        btnSerialize = new QPushButton("Сериализовать в строку");
        
        formLayout->addRow(btnMap);
        formLayout->addRow(btnWhere);
        formLayout->addRow(btnSerialize);

        controlLayout->addLayout(formLayout);
        controlLayout->addStretch();

        textLog = new QTextEdit();
        textLog->setReadOnly(true);
        controlLayout->addWidget(new QLabel("<b>Лог / Результат:</b>"));
        controlLayout->addWidget(textLog);

        scene = new QGraphicsScene(this);
        view = new QGraphicsView(scene);
        view->setRenderHint(QPainter::Antialiasing);
        view->setBackgroundBrush(Qt::lightGray);

        mainLayout->addLayout(controlLayout, 1);
        mainLayout->addWidget(view, 2);

        connect(btnInsert, &QPushButton::clicked, this, &MainWindow::onInsert);
        connect(btnGenerate, &QPushButton::clicked, this, &MainWindow::onGenerate);
        connect(btnTraverse, &QPushButton::clicked, this, &MainWindow::onTraverse);
        connect(btnMap, &QPushButton::clicked, this, &MainWindow::onMap);
        connect(btnWhere, &QPushButton::clicked, this, &MainWindow::onWhere);
        connect(btnSerialize, &QPushButton::clicked, this, &MainWindow::onSerialize);
    }

private slots:
    void onInsert() {
        bool ok;
        int val = inputValue->text().toInt(&ok);
        if (!ok) {
            QMessageBox::warning(this, "Ошибка", "Введите целое число!");
            return;
        }
        tree.insertNode(val);
        inputValue->clear();
        log(QString("Вставлено: %1").arg(val));
        redrawTree();
    }

    void onGenerate() {
        tree = BinaryTree<int>();
        for (int i = 0; i < 20; ++i) {
            tree.insertNode(QRandomGenerator::global()->bounded(1, 100));
        }
        log("Сгенерировано дерево из 20 элементов.");
        redrawTree();
    }

    void onTraverse() {
        QString type = comboTraversal->currentText();
        try {
            Traversal t = traversalFromString(type.toStdString());
            auto vec = tree.traverse(t);
            QStringList list;
            for (int v : vec) list << QString::number(v);
            log(QString("Обход %1:\n%2").arg(type, list.join(", ")));
        } catch (const std::exception& e) {
            log(QString("Ошибка обхода: %1").arg(e.what()));
        }
    }

    void onMap() {
        try {
            auto newTree = tree.map<int>([](int x) { return x * 2; });
            tree = newTree;
            log("Выполнен Map: все элементы умножены на 2.");
            redrawTree();
        } catch (const std::exception& e) {
            log(QString("Ошибка Map: %1").arg(e.what()));
        }
    }

    void onWhere() {
        try {
            auto newTree = tree.where([](const int& x) { return x > 5; });
            tree = newTree;
            log("Выполнен Where: оставлены элементы > 5.");
            redrawTree();
        } catch (const std::exception& e) {
            log(QString("Ошибка Where: %1").arg(e.what()));
        }
    }

    void onSerialize() {
        try {
            QString type = comboTraversal->currentText();
            Traversal t = traversalFromString(type.toStdString());
            std::string s = tree.serialize(t);
            log(QString("Сериализация (%1):\n%2").arg(type, QString::fromStdString(s)));
        } catch (const std::exception& e) {
            log(QString("Ошибка Сериализации: %1").arg(e.what()));
        }
    }

private:
    BinaryTree<int> tree;

    QLineEdit* inputValue;
    QPushButton* btnInsert;
    QPushButton* btnGenerate;
    QComboBox* comboTraversal;
    QPushButton* btnTraverse;
    QPushButton* btnMap;
    QPushButton* btnWhere;
    QPushButton* btnSerialize;
    QTextEdit* textLog;
    QGraphicsScene* scene;
    QGraphicsView* view;

    void log(const QString& msg) {
        textLog->append(msg + "\n");
    }

    void redrawTree() {
        scene->clear();
        if (tree.empty()) return;

        std::function<qreal(Node<int>*, qreal, qreal, qreal)> drawNode = 
            [&](Node<int>* node, qreal x, qreal y, qreal xOffset) -> qreal {
            if (!node) return 0;

            qreal leftWidth = 0;
            qreal rightWidth = 0;
            
            if (node->left) {
                leftWidth = drawNode(node->left, x - xOffset, y + 80, xOffset / 1.5);
            }
            if (node->right) {
                rightWidth = drawNode(node->right, x + xOffset, y + 80, xOffset / 1.5);
            }

            if (node->left) {
                scene->addLine(x, y, x - xOffset, y + 80, QPen(Qt::darkGray));
            }
            if (node->right) {
                scene->addLine(x, y, x + xOffset, y + 80, QPen(Qt::darkGray));
            }

            auto* item = new TreeNodeItem(QString::number(node->value), x, y);
            scene->addItem(item);

            return leftWidth + rightWidth + 50;
        };

        drawNode(tree.root(), 0, 0, 200);
        
        scene->setSceneRect(scene->itemsBoundingRect());
        view->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    }
};

int main(int argc, char *argv[]) {
QApplication app(argc, argv);

app.setStyle("Fusion");

MainWindow window;
window.show();

return app.exec();
}


#include "main.moc"

