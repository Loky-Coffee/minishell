<!DOCTYPE html>
<html lang="de">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
</head>
<body>
    <header>
        <h1>Minishell Dokumentation</h1>
        <p>
            <strong>Minishell</strong> ist eine einfache Shell, die grundlegende Funktionen einer Unix-Shell wie der Bash nachbildet. Dieses Projekt wurde entwickelt, um die Funktionsweise einer Shell besser zu verstehen und die wichtigsten Konzepte der Shell-Programmierung zu vertiefen.
        </p>
        <p>
            Minishell unterstützt viele grundlegende Shell-Kommandos und Konzepte, hat jedoch einige Einschränkungen – insbesondere die Ausführung von Shell-Skripten wird nicht unterstützt.
        </p>
    </header>

  <section>
        <h2>Features</h2>
        <ul>
            <li><strong>Nachbildung der Bash</strong>: Die Minishell verhält sich ähnlich wie die Bash und erlaubt die Eingabe von Kommandos in einer interaktiven Umgebung.</li>
            <li><strong>Umgebungsvariablen</strong>: Unterstützt die Verwaltung und Nutzung von Umgebungsvariablen, wie z.B. <code>$PATH</code>.</li>
            <li><strong>Pipes</strong>: Unterstützt einfache und verkettete Pipes, um die Ausgabe eines Kommandos als Eingabe für ein anderes Kommando zu verwenden.</li>
            <li><strong>Umleitung</strong>: Unterstützt Eingabe- und Ausgabeumleitungen (z.B. <code>&lt;</code>, <code>&gt;</code>).</li>
            <li><strong>Integrierte Befehle</strong>: Befehle wie <code>cd</code>, <code>echo</code>, <code>export</code>, <code>env</code>, <code>pwd</code> und <code>exit</code> sind als buildins eingebaut.</li>
            <li><strong>Signalbehandlung</strong>: Unterstützt die Handhabung von Signalen wie <code>CTRL+C</code> und <code>CTRL+D</code>.</li>
        </ul>
    </section>

  <section>
        <h2>Einschränkungen</h2>
        <p>
            <strong>Keine Unterstützung für Skripte:</strong> Anders als die Bash unterstützt die Minishell <strong>keine Ausführung von Skripten</strong>. Die Shell ist für die interaktive Benutzung vorgesehen.
        </p>
    </section>

  <section>
        <h2>Installation</h2>
        <ol>
            <li><strong>Klonen des Repositories:</strong>
                <pre><code>git clone https://github.com/Loky-Coffee/minishell.git</code></pre>
            </li>
            <li><strong>Kompilieren:</strong>
                <pre><code>cd minishell && make</code></pre>
            </li>
            <li><strong>Ausführen:</strong>
                <pre><code>./minishell</code></pre>
            </li>
        </ol>
    </section>

  <section>
        <h2>Verwendung</h2>
        <p>Nach dem Start von Minishell kannst du Befehle ähnlich wie in der Bash ausführen. Hier sind einige Beispiele:</p>
        <ul>
            <li><strong>Ändern des Verzeichnisses:</strong>
                <pre><code>cd /path/to/directory</code></pre>
            </li>
            <li><strong>Verkettete Kommandos mit Pipes:</strong>
                <pre><code>ls | grep minishell | wc -l</code></pre>
            </li>
            <li><strong>Eingabe- und Ausgabeumleitung:</strong>
                <pre><code>cat &lt; inputfile.txt &gt; outputfile.txt</code></pre>
            </li>
            <li><strong>Beenden der Shell:</strong>
                <pre><code>exit</code></pre>
            </li>
        </ul>
    </section>

  <section>
        <h2>Autor</h2>
        <p>Dieses Projekt wurde von Aristotelis Alatzas(aalatzas) und (nmihaile) entwickelt, um die Konzepte hinter der Shell-Programmierung zu erlernen und eine eigene Version der Bash zu erstellen.</p>
    </section>

<section>
    <h2>Lizenz</h2>
    <p>Dieses Projekt steht unter der <a href="https://github.com/Loky-Coffee/minishell/blob/main/LICENSE">MIT-Lizenz</a>.</p>
</section>
</body>
</html>
