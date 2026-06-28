#include "ExperimentRunner.h"
#include "utils/Timer.h"
#include "utils/Stats.h"
#include "utils/ProgressBar.h"
#include <fstream>
#include <iostream>
#include <algorithm>

GraphResult ExperimentRunner::run(
    const std::string& datasetName,
    const Graph& g,
    const std::vector<std::shared_ptr<IMetric>>& metrics,
    int runs)
{
    GraphResult result;
    result.datasetName = datasetName;
    result.vertices    = g.vertexCount();
    result.edges       = g.edgeCount();
    result.buildTimeMs = 0.0;
    result.memoryBytes = g.memoryBytes();

    for (const auto& metric : metrics) {
        std::vector<double> times;
        times.reserve(runs);

        std::cout << "\n  " << metric->name() << " (" << runs << "x)\n" << std::flush;
        ProgressBar pb;

        for (int i = 0; i < runs; i++) {
            int run = i + 1;
            auto progressFn = [&](int done, int total) {
                pb.update(done, total, run, runs);
            };

            Timer t;
            t.start();
            metric->compute(g, progressFn);
            double ms = t.elapsedMs();
            times.push_back(ms);

            pb.finishRun(run, runs, ms);
        }

        MetricResult mr;
        mr.metricName  = metric->name();
        mr.meanMs      = mean(times);
        mr.varianceMs  = variance(times);
        mr.minMs       = *std::min_element(times.begin(), times.end());
        mr.maxMs       = *std::max_element(times.begin(), times.end());
        result.metrics.push_back(mr);

        std::cout << "  mean=" << mr.meanMs << "ms  var=" << mr.varianceMs
                  << "ms^2  [" << mr.minMs << " - " << mr.maxMs << "]\n";
    }
    return result;
}

void ExperimentRunner::saveCSV(const GraphResult& r, const std::string& filepath) {
    std::ofstream f(filepath);
    f << "dataset,vertices,edges,build_ms,memory_bytes\n";
    f << r.datasetName << "," << r.vertices << "," << r.edges
      << "," << r.buildTimeMs << "," << r.memoryBytes << "\n\n";

    f << "metric,mean_ms,variance_ms,min_ms,max_ms\n";
    for (const auto& m : r.metrics)
        f << m.metricName << "," << m.meanMs << "," << m.varianceMs
          << "," << m.minMs << "," << m.maxMs << "\n";

    std::cout << "  Resultados guardados en " << filepath << "\n";
}
