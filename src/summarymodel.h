#pragma once
#include <QAbstractListModel>

struct Summary
{
    int count;
    int sum;
};

class SummaryModel : public QAbstractListModel
{
    Q_OBJECT
public:
    SummaryModel(QObject* parent = nullptr);
    void setNumbers(const QList<int>& numbers);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(
        int section,
        Qt::Orientation orientation,
        int role = Qt::DisplayRole) const override;

private:
    QList<Summary> m_summaries;
};
