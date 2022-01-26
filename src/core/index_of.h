#ifndef INDEX_OF_H
# define INDEX_OF_H

# define INDEX_OF "<html>" \
					"<head>" \
						"<title>Index of ${path}</title>" \
					"</head>" \
					"<body bgcolor=\"white\">" \
						"<h1>Index of /test/</h1>" \
						"<hr>" \
							"<pre>" \
								"<a href=\"../\">../</a>" \
								"<a href=\"${name}\">${name}</a>                 ${date}            -" \
							"</pre>" \
						"<hr>" \
					"</body>" \
				  "</html>"
#endif
