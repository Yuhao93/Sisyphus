const admin = require('firebase-admin');
const serviceAccount = require('./auth.json');

admin.initializeApp({
	credential: admin.credential.cert(serviceAccount),
	databaseURL: 'https://sisyphus-table.firebaseio.com'
});

exports.getPattern = (id) => {
	return admin.firestore().collection('patterns').doc(id).get();
};
