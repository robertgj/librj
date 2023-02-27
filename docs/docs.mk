include docs/doxygen/doxygen.rules

DOCUMENTATION += librj

.PHONY: cleandocs
cleandocs:
    

.PHONY: gitignore
gitignore:
	-rm -f .gitignore
	echo $(CLEAN_SUFFIXES:%="*"%) > .gitignore
	echo $(CLEAN_FILES) >> .gitignore
	sed -i -e "s/\ /\n/g" .gitignore

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

CLEAN_DIRS += $(addprefix $(DOCS), \
_site .sass-cache .jekyll-cache .jekyll-metadata)

