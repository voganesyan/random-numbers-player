#include "summarymodel.h"

SummaryModel::SummaryModel(QObject* parent) : QAbstractListModel(parent)
{
}

void SummaryModel::setNumbers(const QList<int>& numbers)
{
    beginResetModel();
    m_summaries.clear();
    for (int i = 0; i < numbers.size(); i++)
    {
        m_summaries << Summary{i, numbers[i]};
    }
    endResetModel();
}

int SummaryModel::rowCount(const QModelIndex& parent) const
{
    return m_summaries.size();
}

int SummaryModel::columnCount(const QModelIndex& parent) const
{
    return 2;
}

QVariant SummaryModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_summaries.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        const auto& summary = m_summaries.at(index.row());

        switch (index.column())
        {
            case 0: return summary.count;
            case 1: return summary.sum;
            default: break;
        }
    }
    return QVariant();
}

QVariant SummaryModel::headerData(
    int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
            case 0: return "Count";
            case 1: return "Summary";
            default: break;
        }
    }
    return QVariant();
}
