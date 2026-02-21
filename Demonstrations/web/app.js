// Dashboard renderer for the local data-structures benchmark site
// Loads generated JSON, builds educational cards, and lazily reveals code blocks

const generatedAtNode = document.getElementById("generated-at");
const summaryNode = document.getElementById("summary");
const conceptsNode = document.getElementById("concepts");
const template = document.getElementById("concept-template");

function escapeHtml(input) {
  // Escape dynamic text before placing it into HTML strings
  return input
    .replaceAll("&", "&amp;")
    .replaceAll("<", "&lt;")
    .replaceAll(">", "&gt;")
    .replaceAll('"', "&quot;")
    .replaceAll("'", "&#39;");
}

function makeSummaryCard(label, value) {
  // Summary cards are built with escaped fragments to avoid injection
  const card = document.createElement("article");
  card.className = "summary-card";
  card.innerHTML = `<h3>${escapeHtml(label)}</h3><p>${escapeHtml(value)}</p>`;
  return card;
}

function renderSummary(data) {
  // Reset summary container on each refresh
  summaryNode.innerHTML = "";

  const summary = data.summary || {};
  const successful = summary.successful_concepts ?? 0;
  const total = summary.total_concepts ?? 0;
  const measured = summary.measured_speedups ?? 0;
  const averageSpeedup = typeof summary.average_speedup === "number"
    ? `${summary.average_speedup}x`
    : "n/a";

  let biggest = "n/a";
  const withSpeedups = (data.concepts || [])
    .filter((item) => typeof item.speedup === "number")
    .sort((a, b) => b.speedup - a.speedup);

  if (withSpeedups.length > 0) {
    biggest = `${withSpeedups[0].title} (${withSpeedups[0].speedup}x)`;
  }

  summaryNode.appendChild(makeSummaryCard("Benchmarks Succeeded", `${successful}/${total}`));
  const comparableRatios = successful > 0 ? `${measured}/${successful}` : `${measured}`;
  summaryNode.appendChild(makeSummaryCard("Comparable Ratios", comparableRatios));
  summaryNode.appendChild(makeSummaryCard("Average Ratio", averageSpeedup));
  summaryNode.appendChild(makeSummaryCard("Best Ratio", biggest));
}

function highlightCpp(code) {
  // Lightweight client-side C++ highlighter for readability
  const keywords = [
    "alignas", "alignof", "auto", "bool", "break", "case", "catch", "char", "class", "const",
    "constexpr", "continue", "default", "delete", "do", "double", "else", "enum", "explicit",
    "extern", "false", "float", "for", "friend", "if", "inline", "int", "long", "namespace",
    "new", "noexcept", "nullptr", "operator", "private", "protected", "public", "return",
    "short", "signed", "sizeof", "static", "struct", "switch", "template", "this", "throw",
    "true", "try", "typedef", "typename", "union", "unsigned", "using", "virtual", "void",
    "volatile", "while",
  ];
  const keywordRegex = new RegExp(`\\b(?:${keywords.join("|")})\\b`, "g");
  const tokenRegex = /"(?:\\.|[^"\\])*"|\b\d+\b|\b(?:std|size_t)\b/g;

  function findLineCommentStart(line) {
    // Detect // only when outside quoted strings
    let inString = false;
    let escaped = false;
    for (let i = 0; i < line.length - 1; i += 1) {
      const ch = line[i];
      if (inString) {
        if (escaped) {
          escaped = false;
        } else if (ch === "\\") {
          escaped = true;
        } else if (ch === "\"") {
          inString = false;
        }
      } else if (ch === "\"") {
        inString = true;
      } else if (ch === "/" && line[i + 1] === "/") {
        return i;
      }
    }
    return -1;
  }

  function highlightSegment(segment) {
    // Tokenize code fragment and wrap recognized tokens with classes
    let output = "";
    let lastIndex = 0;
    let match;

    while ((match = tokenRegex.exec(segment)) !== null) {
      output += escapeHtml(segment.slice(lastIndex, match.index)).replace(keywordRegex, '<span class="tok-keyword">$&</span>');
      const token = match[0];
      if (token.startsWith("\"")) {
        output += `<span class="tok-string">${escapeHtml(token)}</span>`;
      } else if (/^\d+$/.test(token)) {
        output += `<span class="tok-number">${token}</span>`;
      } else {
        output += `<span class="tok-type">${escapeHtml(token)}</span>`;
      }
      lastIndex = match.index + token.length;
    }

    output += escapeHtml(segment.slice(lastIndex)).replace(keywordRegex, '<span class="tok-keyword">$&</span>');
    return output;
  }

  return code.split("\n").map((line) => {
    if (line.trimStart().startsWith("#")) {
      return `<span class="tok-preproc">${escapeHtml(line)}</span>`;
    }

    const commentIndex = findLineCommentStart(line);
    if (commentIndex >= 0) {
      const codePart = line.slice(0, commentIndex);
      const commentPart = line.slice(commentIndex);
      return `${highlightSegment(codePart)}<span class="tok-comment">${escapeHtml(commentPart)}</span>`;
    }
    return highlightSegment(line);
  }).join("\n");
}

function renderConcepts(data) {
  // Reset concept list before rendering the latest payload
  conceptsNode.innerHTML = "";

  const concepts = data.concepts || [];

  for (const concept of concepts) {
    if (concept.error) {
      // Render concept-level execution errors without blocking other cards
      const error = document.createElement("article");
      error.className = "error-card";
      error.innerHTML = `<strong>${escapeHtml(concept.title)}</strong><br>${escapeHtml(concept.error)}`;
      conceptsNode.appendChild(error);
      continue;
    }

    const instance = template.content.cloneNode(true);
    // Populate educational text fields first
    instance.querySelector(".concept-title").textContent = concept.title;
    instance.querySelector(".focus-chip").textContent = concept.focus;

    instance.querySelector('[data-field="definition"]').textContent = concept.education.definition;
    instance.querySelector('[data-field="used-for"]').textContent = concept.education.used_for.join(", ");

    instance.querySelector('[data-field="case"]').textContent = concept.case.real_world_case;
    instance.querySelector('[data-field="problem"]').textContent = concept.case.problem;
    instance.querySelector('[data-field="pain"]').textContent = concept.case.pain_point;

    instance.querySelector('[data-field="why-unoptimized"]').textContent = concept.education.why_unoptimized;
    instance.querySelector('[data-field="small-change"]').textContent = concept.education.small_change;

    instance.querySelector('[data-field="unoptimized-structure"]').textContent = concept.unoptimized_structure;
    instance.querySelector('[data-field="optimized-structure"]').textContent = concept.optimized_structure;
    instance.querySelector('[data-field="before-complexity"]').textContent = concept.education.before_complexity;
    instance.querySelector('[data-field="after-complexity"]').textContent = concept.education.after_complexity;

    const unoptimizedUs = Number(concept.unoptimized.elapsed_us);
    const optimizedUs = Number(concept.optimized.elapsed_us);

    instance.querySelector('[data-field="unoptimized-ms"]').textContent = concept.unoptimized.elapsed_display ?? "n/a";
    instance.querySelector('[data-field="optimized-ms"]').textContent = concept.optimized.elapsed_display ?? "n/a";

    // Per-card normalization keeps each concept comparison visually accurate
    const safeUnoptimizedUs = Number.isFinite(unoptimizedUs) && unoptimizedUs >= 0 ? unoptimizedUs : 0;
    const safeOptimizedUs = Number.isFinite(optimizedUs) && optimizedUs >= 0 ? optimizedUs : 0;
    const localMax = Math.max(safeUnoptimizedUs, safeOptimizedUs, 0);
    const unoptimizedWidth = localMax > 0 ? (safeUnoptimizedUs / localMax) * 100 : 0;
    const optimizedWidth = localMax > 0 ? (safeOptimizedUs / localMax) * 100 : 0;

    instance.querySelector(".bar-unoptimized").style.width = `${unoptimizedWidth}%`;
    instance.querySelector(".bar-optimized").style.width = `${optimizedWidth}%`;

    if (safeUnoptimizedUs > 0 && safeOptimizedUs > 0) {
      // Derive comparison text from high-resolution runtime values
      const computedSpeedup = safeUnoptimizedUs / safeOptimizedUs;
      const percentReduction = ((safeUnoptimizedUs - safeOptimizedUs) / safeUnoptimizedUs) * 100;
      instance.querySelector('[data-field="speedup"]').textContent =
        `Measured result: ${computedSpeedup.toFixed(2)}x faster (${percentReduction.toFixed(1)}% less time)`;
    } else {
      instance.querySelector('[data-field="speedup"]').textContent = "Measured result: speedup unavailable for this run";
    }

    const codeDetails = instance.querySelector(".code-details");
    const unoptimizedCodeNode = instance.querySelector('[data-field="unoptimized-code"]');
    const optimizedCodeNode = instance.querySelector('[data-field="optimized-code"]');

    // Defer large code block rendering until the panel is opened
    codeDetails.addEventListener("toggle", () => {
      if (!codeDetails.open || codeDetails.dataset.loaded === "1") {
        return;
      }
      unoptimizedCodeNode.innerHTML = highlightCpp(concept.source.unoptimized);
      optimizedCodeNode.innerHTML = highlightCpp(concept.source.optimized);
      codeDetails.dataset.loaded = "1";
    });

    // Keep raw stdout available for audit/debug context
    const rawOutput = [
      "[Unoptimized]",
      concept.unoptimized.raw_output,
      "",
      "[Optimized]",
      concept.optimized.raw_output,
    ].join("\n");
    instance.querySelector('[data-field="raw-output"]').textContent = rawOutput;

    conceptsNode.appendChild(instance);
  }
}

async function init() {
  try {
    // Cache-bust query param ensures browser fetches fresh benchmark payload
    const response = await fetch(`./data/results.json?t=${Date.now()}`);
    if (!response.ok) {
      throw new Error(`Failed to load results.json (${response.status})`);
    }

    const data = await response.json();
    const generatedAt = new Date(data.generated_at);
    generatedAtNode.textContent = `Generated: ${generatedAt.toLocaleString()}`;

    renderSummary(data);
    renderConcepts(data);
  } catch (error) {
    generatedAtNode.textContent = "Failed to load benchmark results";
    const message = document.createElement("article");
    message.className = "error-card";
    message.innerHTML = [
      "<strong>Dashboard Load Failed</strong>",
      `<div>${escapeHtml(String(error))}</div>`,
      "<div>Run <code>./run_web_demo.sh</code> to regenerate data/results.json</div>",
    ].join("");
    conceptsNode.appendChild(message);
  }
}

init();
