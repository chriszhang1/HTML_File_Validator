# HTML File Validator

HTML File Validator is a program that determines if a given HTML file is in the correct format. It has a C version and a Haskell version.

## Features

- Confirm that each tag is a valid HTML tag.
- Check that each tag has a corresponding closing tag if needed.
- Check that the tags are nested properly.
- Check that there are <html> and </html> tags around the entire document.
- Check that there is a single <head>…</head> section followed by a single <body>…</body> section in that order.

## Constraints

- Program only works with tags in this set: {html, head, body, title, h1, h2, h3, p, ul, li, a, div, br, hr}
- A <div> tag cannot be nested inside a <p> tag, and a <p> tag cannot be nested inside a <p> tag. A <div> tag can nest inside another <div>.
- <br> is one of the few tags that doesn’t have a closing tag, so documents can contain just <br>. Similarly for <hr>.
- Any attributes in an opening tag are ignored. For example, in <div class=”text”> the class attribute is ignored.
- The <title> tag is included in the head section between the head tags only, and not in the body section between the body tags.
- A DOCTYPE at the start of the document is not required.
- Assume there are no comments (<!-- -->) or entities (e.g., &lt; to represent <) in the HTML document.
- Assume that the ‘<’ and ‘>’ characters are used only as part of the HTML tags, and do not appear in the text within p, h1, h2, h3, or any other sections, or in attribute values.
- The input file name should be fixed as ‘file.html’.


## Example

An example of a complete valid HTML file based on the description above:

```
<html>
    <head>
        <title>Example Document</title>
    </head>
    <body>
        <h1>A Title</h1>
        <div class="content">
            <p>A paragraph with words and sentences.</p>
            <br>
            <h2>A SubTitle</h2>
            <p>More text and a link: <a href="https://www.github.com">GitHub</a></p>
            <hr>
            <ul>
                <li><p>List item 1</p></li>
                <li><p>List item 2</p></li>
            </ul>
            <div class="footer">
                <p>This is the footer text.</p>
            </div>
        </div>
    </body>
</html>
```
