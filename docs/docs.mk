include docs/doxygen/doxygen.rules

DOCUMENTATION += librj

.PHONY: cleandocs
cleandocs:
    

#
# For github and jekyll
#
JEKYLL_OPTS=--config $(DOCS)/_config.yml --source $(DOCS) \
--destination $(DOCS)/_site

.PHONY: cleanjekyll
cleanjekyll:    
	jekyll clean $(JEKYLL_OPTS)

.PHONY: jekyll
jekyll: clean cleanjekyll
	jekyll serve $(JEKYLL_OPTS)

CLEAN_DIRS += $(addprefix $(DOCS)/, \
_site .sass-cache .jekyll-cache .jekyll-metadata)

