release:
	sh build.sh Release64

debug:
	sh build.sh Debug64

clean:
	rm -rf .cache
	rm -rf deps
	rm -rf R-link/r4aimms/target/
	rm -f results.txt
	rm -f RFunctionalModel.lis
	rm -f R-link/aimms4r/src/aimms4r.so
	rm -rf target
	rm -f runFunctionalModel.elg
	rm -f runFunctionalModel.err
	rm -f runFunctionalModel.blg
	rm -f runFunctionalModel.xlg
	rm -f runFunctionalModel.usageLog
	rm -f runFunctionalModelDebug.elg
	rm -f runFunctionalModelDebug.err
	rm -f runFunctionalModelDebug.blg
	rm -f runFunctionalModelDebug.xlg
	rm -f runFunctionalModelDebug.usageLog
	rm -f *.pyc
