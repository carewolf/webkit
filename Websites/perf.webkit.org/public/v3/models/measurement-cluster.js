
class MeasurementCluster {
    constructor(response)
    {
        this._response = response;
        this._adaptor = new MeasurementAdaptor(response['formatMap']);
    }

    startTime() { return this._response['startTime']; }

    addToSeries(series, configType, includeOutliers, idMap)
    {
        var rawMeasurements = this._response['configurations'][configType];
        if (!rawMeasurements)
            return;

        var self = this;
        rawMeasurements.forEach(function (row) {
            var id = self._adaptor.extractId(row);
            if (id in idMap)
                return;
            if (row[self._markedOutlierIndex] && !includeOutliers)
                return;

            idMap[id] = true;

            series._series.push(self._adaptor.adoptToSeries(row, series, series._series.length));
        });
    }
}
